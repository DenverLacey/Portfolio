#include "Polygon.h"

#include <Gizmos.h>


Polygon::Polygon(const std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& colour, float rotation, float angularVelocity, float mass, float drag, float angularDrag, float elasticity)
	:	Rigidbody(ShapeType::POLY, position, velocity, rotation, angularVelocity, mass, drag, angularDrag, elasticity, colour),
		m_relativeVertices(vertices)
{
	for (int i = 0; i < vertices.size(); i++) {
		m_vertices.push_back(vertices[i]);
	}
}

Polygon::~Polygon()
{
}

void Polygon::fixedUpdate(const glm::vec2& gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	// update corners' position
	for (int i = 0; i < m_vertices.size(); i++) {
		m_vertices[i] = m_position + m_relativeVertices.at(i);
	}
}

void Polygon::makeGizmo() const
{
	if (m_vertices.size() >= 3) {
		for (int i = 1; i < m_vertices.size(); i++) {
			aie::Gizmos::add2DLine(m_vertices.at(i - 1), m_vertices.at(i), m_colour);
		}
		aie::Gizmos::add2DLine(m_vertices.at(m_vertices.size() - 1), m_vertices.at(0), m_colour);
	}
}

void Polygon::resetPosition()
{
	m_position = glm::vec2(0, 0);
}

const std::vector<glm::vec2>& Polygon::getVertices() const
{
	return m_vertices;
}

void Polygon::debug() const
{
}
