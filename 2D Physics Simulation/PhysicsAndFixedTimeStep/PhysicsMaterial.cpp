#include "PhysicsMaterial.h"



const float PhysicsMaterial::MIN_THRESHOLD = 0.1f;

PhysicsMaterial::PhysicsMaterial()
	:	m_staticCoefficient(0.0f),
		m_kineticCoefficient(0.0f),
		m_asleep(true),
		m_other(nullptr)
{}

PhysicsMaterial::~PhysicsMaterial()
{
}

PhysicsMaterial::PhysicsMaterial(const float staticCoefficient, const float kineticCoefficient)
	:	m_staticCoefficient(staticCoefficient),
		m_kineticCoefficient(kineticCoefficient),
		m_asleep(true),
		m_other(nullptr)
{}



void PhysicsMaterial::applyStaticFriction(glm::vec2* const outForce)
{
	if (m_asleep && m_other != nullptr) {
		float friction = staticFriction();
		glm::vec2 frictionForce = *outForce * friction;
		*outForce -= frictionForce;

		if (glm::length(*outForce) < MIN_THRESHOLD) {
			*outForce = glm::vec2(0, 0);
		}
	}
}

void PhysicsMaterial::applyKineticFriction(glm::vec2* const outVelocity)
{
	if (!m_asleep && m_other != nullptr) {
		float friction = kineticFriction();
		glm::vec2 frictionForce = *outVelocity * friction;
		*outVelocity -= frictionForce;

		if (glm::length(*outVelocity) < MIN_THRESHOLD) {
			*outVelocity = glm::vec2(0, 0);
		}
	}
}

void PhysicsMaterial::setAsleep(const bool state)
{
	m_asleep = state;
}

void PhysicsMaterial::establishContact(const PhysicsMaterial* other, const glm::vec2& contactNormal)
{
	m_other = other;
	m_contactNormal = contactNormal;
}

void PhysicsMaterial::disestablishContact()
{
	m_other = nullptr;
}

const float PhysicsMaterial::staticFriction() const
{
	float coefficient = (m_staticCoefficient + m_other->m_staticCoefficient) / 2.f;
	return coefficient * glm::length(m_contactNormal);
}

const float PhysicsMaterial::kineticFriction() const
{
	return (m_kineticCoefficient + m_other->m_kineticCoefficient) / 2.f;
}

