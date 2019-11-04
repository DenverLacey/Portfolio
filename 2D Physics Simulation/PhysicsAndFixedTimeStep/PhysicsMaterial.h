#pragma once
#include <glm/ext.hpp>

class PhysicsMaterial
{
public:
	PhysicsMaterial();
	~PhysicsMaterial();

	PhysicsMaterial(const float staticCoefficient, const float kineticCoefficient);

public:
	void applyStaticFriction(glm::vec2* const outForce);
	void applyKineticFriction(glm::vec2* const outVelocity);

	void setAsleep(const bool state);

	void establishContact(const PhysicsMaterial* other, const glm::vec2& contactNormal);
	void disestablishContact();

private:
	const float staticFriction() const;
	const float kineticFriction() const;
	
public:
	const static float MIN_THRESHOLD;

private:
	float m_staticCoefficient;
	float m_kineticCoefficient;
	bool m_asleep;

	const PhysicsMaterial* m_other;
	glm::vec2 m_contactNormal;
};

