#include "Sphere.h"

#include <iostream>
#include <Gizmos.h>



Sphere::Sphere(const glm::vec2& position, const glm::vec2& velocity, float radius, float mass, float rotation, float angularVelocity, float drag, float angularDrag, float elasticity, const glm::vec4& colour)
	: Rigidbody(ShapeType::SPHERE, position, velocity, rotation, angularVelocity, mass, drag, angularDrag, elasticity, colour), m_radius(radius)
{
	m_moment = 0.5f * mass * radius * radius;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo() const {
	aie::Gizmos::add2DCircle(m_position, m_radius, 16, m_colour);
}

float Sphere::getRadius() const {
	return m_radius;
}

glm::vec4 Sphere::getColour() const {
	return m_colour;
}

void Sphere::debug() const {
	std::cout << "Shape ID: Sphere, Rdaius: " << m_radius << "\n";
	std::cout << m_mass;
}
