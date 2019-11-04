#include "Rigidbody.h"
#include "CollisionInfo.h"
#include "Polygon.h"

const float Rigidbody::MIN_LINEAR_THRESHOLD = 0.1f;
const float Rigidbody::MIN_ROTATION_THRESHOLD = 0.01f;

Rigidbody::Rigidbody(ShapeType shapeID, const glm::vec2& position, const glm::vec2& velocity, float rotation, float angularVelocity, float mass, float drag, float angularDrag, float elasticity, const glm::vec4& colour)
	:	PhysicsObject(shapeID), m_position(position), m_velocity(velocity), 
		m_rotation(rotation), m_angularVelocity(angularVelocity), m_mass(mass), m_drag(drag), 
		m_angularDrag(angularDrag), m_elasticity(elasticity),
		m_isStatic(false), m_colour(colour)
{}

Rigidbody::~Rigidbody() 
{}



void Rigidbody::fixedUpdate(const glm::vec2& gravity, float timeStep) {
	
	if (!m_isStatic) {
		applyForce(gravity * m_mass * timeStep);
		m_position += m_velocity * timeStep;

		m_rotation += m_angularVelocity * timeStep;

		// apply drag
		m_velocity -= m_velocity * m_drag * timeStep;
		m_angularVelocity -= m_angularDrag * timeStep;

		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) {
			m_velocity = glm::vec2(0, 0);
		}
		if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD) {
			m_angularVelocity = 0.f;
		}

		// apply friction
		m_physicsMaterial.applyKineticFriction(&m_velocity);

		if (glm::length(m_velocity) < PhysicsMaterial::MIN_THRESHOLD) {
			m_physicsMaterial.setAsleep(true);
		}
		else {
			m_physicsMaterial.setAsleep(false);
		}
	}
}

void Rigidbody::applyForce(glm::vec2& force) {
	
	// apply friction
	m_physicsMaterial.applyStaticFriction(&force);

	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2& force) {

	// apply friction
	m_physicsMaterial.applyStaticFriction(&force);

	actor2->applyForce(force);
	applyForce(-force);
}

void Rigidbody::applyAngularForce(const glm::vec2& force, const glm::vec2& position)
{
	m_angularVelocity += (force.y * position.x - force.x * position.y) / m_moment;
}

void Rigidbody::resolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, const CollisionInfo& info)
{
	Rigidbody* a = dynamic_cast<Rigidbody*>(obj1);
	Rigidbody* b = dynamic_cast<Rigidbody*>(obj2);

	if (!a && !b) { return; }

	if (!a || !b) {
		resolveDynamicByStatic((a ? a : b), info);
	}
	else if (!a->getStatic() && !b->getStatic()){
		resolveDynamicByDynamic(a, b, info);
	}
	else {
		resolveDynamicByStatic((a->getStatic() ? b : a), info);
	}
}

void Rigidbody::resolveDynamicByDynamic(Rigidbody* a, Rigidbody* b, const CollisionInfo& info)
{
	glm::vec2 relVelocity = b->getVelocity() - a->m_velocity;

	float elasticity = (a->m_elasticity + b->m_elasticity) / 2.f;

	float j = glm::dot(-(1 + elasticity) * (relVelocity), info.normal) /
		glm::dot(info.normal, info.normal * ((1 / a->m_mass) + (1 / b->getMass())));

	glm::vec2 force = info.normal * j;
	a->applyForceToActor(b, force);
}

void Rigidbody::resolveDynamicByStatic(Rigidbody* rb, const CollisionInfo& info)
{
	glm::vec2 relVelocity = rb->m_velocity;

	float elasticity = rb->m_elasticity;
	float j = glm::dot(-(1 + elasticity) * (relVelocity), info.normal) /
		(1 / rb->m_mass);

	glm::vec2 force = info.normal * j;
	rb->applyForce(force);
}

void Rigidbody::displace(const glm::vec2& displacement)
{
	m_position += displacement;
}

glm::vec2 Rigidbody::getPosition() const {
	return m_position;
}

float Rigidbody::getRotation() const {
	return m_rotation;
}

glm::vec2 Rigidbody::getVelocity() const {
	return m_velocity;
}

float Rigidbody::getMass() const {
	return m_mass;
}

float Rigidbody::getDrag() const
{
	return m_drag;
}

float Rigidbody::getAngularDrag() const
{
	return m_angularDrag;
}

float Rigidbody::getElasticity() const
{
	return m_elasticity;
}

void Rigidbody::setMass(float mass)
{
	m_mass = mass;
}

void Rigidbody::setDrag(float drag)
{
	m_drag = drag;
}

void Rigidbody::setAngularDrag(float angularDrag)
{
	m_angularDrag = angularDrag;
}

const bool Rigidbody::getStatic() const
{
	return m_isStatic;
}

void Rigidbody::setStatic(bool state)
{
	m_isStatic = state;
}

