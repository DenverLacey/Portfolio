#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(const glm::vec2& normal, float distance);
	~Plane();

	void fixedUpdate(const glm::vec2& gravity, float timeStep) override;
	void debug() const override;
	void makeGizmo() const override;
	void resetPosition() override;

	const glm::vec2& getNormal() const;
	float getDistance() const;

protected:
	glm::vec2 m_normal;
	float m_distance;
};

