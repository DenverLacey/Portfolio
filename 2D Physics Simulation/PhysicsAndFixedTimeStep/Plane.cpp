#include "Plane.h"

#include <iostream>
#include <Gizmos.h>

#include "PhysicsMaterial.h"

Plane::Plane()
	: PhysicsObject(ShapeType::PLANE)
{
}

Plane::Plane(const glm::vec2 & normal, float distance)
	: PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distance(distance)
{}

Plane::~Plane()
{}



void Plane::fixedUpdate(const glm::vec2& gravity, float timeStep)
{
}

void Plane::debug() const
{
	std::cout << "Shape ID: Plane, " <<
		"Normal: " << "(" << m_normal.x << ", " << m_normal.y << "), " <<
		"Distance: " << m_distance << std::endl;
}

void Plane::makeGizmo() const
{
	float length = 300;
	glm::vec2 centrePoint = m_normal * m_distance;
		// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 start(centrePoint + (parallel * length));
	glm::vec2 end = centrePoint - (parallel * length);
	glm::vec4 colour(1, 1, 1, 1);

	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::resetPosition()
{
	m_distance = 0.f;
}

const glm::vec2 & Plane::getNormal() const
{
	return m_normal;
}

float Plane::getDistance() const
{
	return m_distance;
}
