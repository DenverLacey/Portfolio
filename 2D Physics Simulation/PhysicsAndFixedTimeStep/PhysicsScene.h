#pragma once
#include <glm/ext.hpp>
#include <vector>
#include "CollisionInfo.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

public:
	void addActor(PhysicsObject* actor);
	bool removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();

	void checkCollision();

	static CollisionInfo plane2Plane(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo plane2Sphere(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo plane2Poly(PhysicsObject* a, PhysicsObject* b);
	
	static CollisionInfo sphere2Plane(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo sphere2Sphere(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo sphere2Poly(PhysicsObject* a, PhysicsObject* b);

	static CollisionInfo poly2Plane(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo poly2Sphere(PhysicsObject* a, PhysicsObject* b);
	static CollisionInfo poly2Poly(PhysicsObject* a, PhysicsObject* b);

	void setGravity(const glm::vec2& gravity);
	const glm::vec2& getGravity() const;

	void setTimeStep(const float timeStep);
	float getTimeStep() const;

	void debugScene() const;

private:
	static const glm::vec2& project(const glm::vec2& axis, const std::vector<glm::vec2>& verts);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;


	typedef CollisionInfo(*func)(PhysicsObject*, PhysicsObject*);
	func m_collisionFunctionPntrs[9] {
		plane2Plane,
		plane2Sphere,
		plane2Poly,

		sphere2Plane,
		sphere2Sphere,
		sphere2Poly,

		poly2Plane,
		poly2Sphere,
		poly2Poly
	};
};

