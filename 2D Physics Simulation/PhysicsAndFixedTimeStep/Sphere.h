#pragma once
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
	Sphere(	const glm::vec2& position, const glm::vec2& velocity, float radius, 
			float mass, float rotation = 0.0f, float angularVelocity = 0.0f, float drag = 0.3f, float angularDrag = 0.3f, 
			float elasticity = 1.f, const glm::vec4& colour = glm::vec4(1, 1, 1, 1));
	~Sphere();

public:
	void makeGizmo() const override;

	float getRadius() const;
	glm::vec4 getColour() const;

	void debug() const override;

protected:
	float m_radius;
};

