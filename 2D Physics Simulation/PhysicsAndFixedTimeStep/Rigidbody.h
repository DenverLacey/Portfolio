#pragma once
#include "PhysicsObject.h"
#include <vector>

class CollisionInfo;

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, const glm::vec2& position, const glm::vec2& velocity, float rotation, float angularVelocity, float mass, float drag = 0.3f, float angularDrag = 0.3f, float elasticity = 1.f, const glm::vec4& colour = glm::vec4(1, 1, 1, 1));
	virtual ~Rigidbody();

public:
	void fixedUpdate(const glm::vec2& gravity, float timeStep) override;
	
	void applyForce(glm::vec2& force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2& force);
	void applyAngularForce(const glm::vec2& force, const glm::vec2& position);

	static void resolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, const CollisionInfo& info);

	void displace(const glm::vec2& displacement);

	glm::vec2 getPosition() const;
	float getRotation() const;
	glm::vec2 getVelocity() const;

	float getMass() const;
	float getDrag() const;
	float getAngularDrag() const;
	float getElasticity() const;

	void setMass(float mass);
	void setDrag(float drag);
	void setAngularDrag(float angularDrag);

	const bool getStatic() const;
	void setStatic(bool state);

private:
	static void resolveDynamicByDynamic(Rigidbody* a, Rigidbody* b, const CollisionInfo& info);
	static void resolveDynamicByStatic(Rigidbody* rb, const CollisionInfo& info);

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_rotation;	// 2D so we only need a single float to represent our rotation
	float m_angularVelocity;
	float m_moment;		// moment of inertia, rotation equivalent of mass

	float m_mass;
	float m_drag;
	float m_angularDrag;
	float m_elasticity;

	bool m_isStatic;

	glm::vec4 m_colour;

private:
	static const float MIN_LINEAR_THRESHOLD;
	static const float MIN_ROTATION_THRESHOLD;
};

