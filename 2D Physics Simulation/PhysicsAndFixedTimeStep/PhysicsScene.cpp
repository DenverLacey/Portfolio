#include "PhysicsScene.h"

#include <iostream>
#include <algorithm>
#include <vector>

#include <Gizmos.h>

#include "Rigidbody.h"
#include "Plane.h"
#include "Sphere.h"
#include "AABB.h"

PhysicsScene::PhysicsScene() 
	: m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene() {

	for (PhysicsObject* pActor : m_actors) {
		if (pActor != nullptr) {
			delete pActor;
			pActor = nullptr;
		}
	}
}



void PhysicsScene::addActor(PhysicsObject* actor) {
	m_actors.push_back(actor);
}

bool PhysicsScene::removeActor(PhysicsObject* actor) {
	auto itr = std::find(m_actors.begin(), m_actors.end(), actor);
	if (*itr == actor) {
		m_actors.erase(itr);
		return true;
	}
	return false;
}

void PhysicsScene::update(float dt) {

	// update physics at fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep) {
		for (PhysicsObject* pActor : m_actors) {
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
		
		// check for collisions (ideally you'd want some sort of scene managment in place)
		checkCollision();
	}
}

void PhysicsScene::updateGizmos() {
	for (PhysicsObject* pActor : m_actors) {
		pActor->makeGizmo();
	}
}

void PhysicsScene::checkCollision()
{
	size_t actorCount = m_actors.size();

	// need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++) {
		for (int inner = outer + 1; inner < actorCount; inner++) {
			PhysicsObject* a = m_actors.at(outer);
			PhysicsObject* b = m_actors.at(inner);
			int aID = (int)a->getShapeID();
			int bID = (int)b->getShapeID();

			// using function pointers
			int funcIndex = (aID * (int)ShapeType::SHAPE_COUNT) + bID;
			func collisionFnPtr = m_collisionFunctionPntrs[funcIndex];

			if (collisionFnPtr != nullptr) {
			CollisionInfo collisionInfo = (*collisionFnPtr)(a, b);

			//	// if collision
				if (collisionInfo != CollisionInfo::zero) {
					// friction
					a->establishContact(&b->getPhysicsMaterial(), collisionInfo.normal);
					b->establishContact(&a->getPhysicsMaterial(), collisionInfo.normal);

					Rigidbody::resolveCollision(a, b, collisionInfo);
				}
				else {
					a->diestablishContact();
					b->diestablishContact();
				}
			}
		}
	}
}

CollisionInfo PhysicsScene::plane2Plane(PhysicsObject* a, PhysicsObject* b)
{
	return CollisionInfo::zero;
}

CollisionInfo PhysicsScene::plane2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	Plane* plane = dynamic_cast<Plane*>(a);
	Sphere* sphere = dynamic_cast<Sphere*>(b);
		
	CollisionInfo info = CollisionInfo::zero;

	if (plane != nullptr && sphere != nullptr) {
		glm::vec2 normal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			normal) - plane->getDistance();

		float intersection = sphere->getRadius() - sphereToPlane;

		if (intersection > 0) {
			sphere->displace(normal * intersection);
			info.normal = normal;
		}
	}
	return info;
}

CollisionInfo PhysicsScene::plane2Poly(PhysicsObject* a, PhysicsObject* b)
{
	Plane* plane = dynamic_cast<Plane*>(a);
	Polygon* poly = dynamic_cast<Polygon*>(b);

	CollisionInfo info = CollisionInfo::zero;

	if (plane && poly) {
		std::vector<glm::vec2> verts = poly->getVertices();
		glm::vec2 normal = plane->getNormal();
		float planeDistance = plane->getDistance();

		float overlap = INFINITY;
		for (glm::vec2& v : verts) {
			float dot = glm::dot(normal, v) - planeDistance;
			if (dot < overlap) {
				overlap = dot;
			}
		}

		// if collision
		if (overlap < 0) {
			poly->displace(normal * -overlap);
			info.normal = normal;
		}
	}
	return info;
}

CollisionInfo PhysicsScene::sphere2Plane(PhysicsObject* a, PhysicsObject* b)
{
	return plane2Sphere(b, a);
}

CollisionInfo PhysicsScene::sphere2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(a);
	Sphere* sphere2 = dynamic_cast<Sphere*>(b);

	CollisionInfo info = CollisionInfo::zero;

	if (sphere1 != nullptr && sphere2 != nullptr) {
		float distance = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		float combinedRadius = sphere1->getRadius() + sphere2->getRadius();

		if (distance <= combinedRadius) {
			info.normal = glm::normalize(sphere2->getPosition() - sphere1->getPosition());
			float overlap = combinedRadius - distance;

			if (!sphere2->getStatic()) { sphere2->displace(info.normal * overlap); }
			else if (!sphere1->getStatic()) { sphere1->displace(info.normal * -overlap); }
		}
	}
	return info;
}

CollisionInfo PhysicsScene::sphere2Poly(PhysicsObject* a, PhysicsObject* b)
{
	Sphere* sphere = dynamic_cast<Sphere*>(a);
	Polygon* poly = dynamic_cast<Polygon*>(b);

	CollisionInfo info = CollisionInfo::zero;

	if (sphere && poly) {
		std::vector<glm::vec2> polyVerts = poly->getVertices();
		glm::vec2 spherePos = sphere->getPosition();
		float sphereRadius = sphere->getRadius();

		// calculate normals of poly
		std::vector<glm::vec2> norms;
		for (int i = 1; i <= polyVerts.size(); i++) {
			int index = i % polyVerts.size();
			glm::vec2 normal = glm::normalize(polyVerts[index] - polyVerts[i - 1]);
			normal = { -normal.y, normal.x };
			norms.push_back(normal);
		}

		float overlap = INFINITY;
		glm::vec2 smallest;

		// loop over norms
		for (glm::vec2& axis : norms) {
			// project both to axis
			const glm::vec2 p1 = project(axis, polyVerts);
			
			glm::vec2 min = spherePos - sphereRadius * axis;
			glm::vec2 max = spherePos + sphereRadius * axis;

			const glm::vec2 p2 = project(axis, { min, max });

			// check for overlap
			if (!(p1.x < p2.y && p1.y > p2.x)) // if no overlap
				return info;
			else {
				float o = (p1.y > p2.x ? p1.y - p2.x : p2.y - p1.x);
				if (o < overlap) {
					overlap = o;
					smallest = axis;
				}
			}
		}

		// if this far, collision occurred
		if (!sphere->getStatic()) { sphere->displace(smallest * overlap); }
		else if (!poly->getStatic()) { poly->displace(smallest * -overlap); }
		
		info.normal = smallest;
	}
	return info;
}

CollisionInfo PhysicsScene::poly2Plane(PhysicsObject* a, PhysicsObject* b)
{
	return plane2Poly(b, a);
}

CollisionInfo PhysicsScene::poly2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	return sphere2Poly(b, a);
}

CollisionInfo PhysicsScene::poly2Poly(PhysicsObject* a, PhysicsObject* b)
{
	Polygon* poly1 = dynamic_cast<Polygon*>(a);
	Polygon* poly2 = dynamic_cast<Polygon*>(b);

	CollisionInfo info = CollisionInfo::zero;

	if (poly1 && poly2) {
		// get vertices of a and b
		std::vector<glm::vec2> aVerts = poly1->getVertices();
		std::vector<glm::vec2> bVerts = poly2->getVertices();

		// calculate normals for a and b
		std::vector<glm::vec2> aNorms;
		for (int i = 1; i <= aVerts.size(); i++) {
			int index = i % aVerts.size();
			glm::vec2 normal = glm::normalize(aVerts[index] - aVerts[i - 1]);
			normal = { -normal.y, normal.x };
			aNorms.push_back(normal);
		}

		std::vector<glm::vec2> bNorms;
		for (int i = 1; i <= bVerts.size(); i++) {
			int index = i % bVerts.size();
			glm::vec2 normal = glm::normalize(bVerts[index] - bVerts[i - 1]);
			normal = { -normal.y, normal.x };
			bNorms.push_back(normal);
		}

		float overlap = INFINITY;
		glm::vec2 smallest;

		// loop over aNorms
		for (glm::vec2& axis : aNorms) {
			// project both shapes onto axis
			const glm::vec2 p1 = project(axis, aVerts);
			const glm::vec2 p2 = project(axis, bVerts);

			// check for overlap
			if (!(p1.x < p2.y && p1.y > p2.x))
				return info;
			else {
				float o = (p1.y > p2.x ? p1.y - p2.x : p2.y - p1.x);
				if (o < overlap) {
					overlap = o;
					smallest = axis;
				}
			}
		}

		// loop over bNorms
		for (glm::vec2& axis : bNorms) {
			// project both shapes onto axis
			glm::vec2 p1 = project(axis, aVerts);
			glm::vec2 p2 = project(axis, bVerts);

			// check for overlap
			if (!(p1.x < p2.y && p1.y > p2.x))
				return info;
			else {
				float o = (p1.y > p2.x ? p1.y - p2.x : p2.y - p1.x);
				if (o < overlap) {
					overlap = o;
					smallest = axis;
				}
			}
		}

		// if this far, collision occured
		if (!poly1->getStatic() && !poly2->getStatic()) {
			poly1->displace(smallest * -overlap);
			poly2->displace(smallest * overlap);
		}
		else if (!poly1->getStatic()) { poly1->displace(smallest * -overlap * 2.f); }
		else if (!poly2->getStatic()) { poly2->displace(smallest * overlap * 2.f); }

		info.normal = smallest;
	}
	return info;
}

void PhysicsScene::setGravity(const glm::vec2& gravity) {
	m_gravity = gravity;
}

const glm::vec2& PhysicsScene::getGravity() const {
	return m_gravity;
}

void PhysicsScene::setTimeStep(const float timeStep) {
	m_timeStep = timeStep;
}

float PhysicsScene::getTimeStep() const {
	return m_timeStep;
}

void PhysicsScene::debugScene() const
{
	int count = 0;
	for (PhysicsObject* pActor : m_actors) {
		std::cout << count << ": ";
		pActor->debug();
		count++;
	}
}

const glm::vec2 & PhysicsScene::project(const glm::vec2 & axis, const std::vector<glm::vec2>& verts)
{
	float min = glm::dot(axis, verts.at(0));
	float max = min;
	for (int i = 1; i < verts.size(); i++) {
		float p = glm::dot(axis, verts.at(i));

		if (p < min)
			min = p;
		else if (p > max)
			max = p;
	}
	glm::vec2 proj(min, max);
	return proj;
}
