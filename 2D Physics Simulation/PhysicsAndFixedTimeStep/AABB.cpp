#include "AABB.h"

#include <Gizmos.h>
#include <iostream>

AABB::AABB()
	: Polygon({
		{-1, -1},
		{-1, 1},
		{1, 1},
		{1, -1}
		})
{}


AABB::~AABB()
{
}

AABB::AABB(const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& extents, float mass, float drag, float angularDrag, float elasticity, const glm::vec4& colour)
	: Polygon({-extents, {-extents.x, extents.y}, extents, {extents.x, -extents.y}},
		position, velocity, colour, 0, 0, mass, drag, angularDrag, elasticity), m_extents(extents)
{
	m_moment = 1.f / 12.f * mass * extents.x * extents.y;
}


void AABB::makeGizmo() const
{
	aie::Gizmos::add2DAABB(m_position, m_extents, m_colour);
}

const glm::vec2 AABB::getMin() const
{
	return m_position - m_extents;
}

const glm::vec2 AABB::getMax() const
{
	return m_position + m_extents;
}

const glm::vec2& AABB::getExtents() const
{
	return m_extents;
}

void AABB::debug() const
{
	std::cout << "Shape ID: Box, " <<
		"HalfExtents: (" << m_extents.x << ", " << m_extents.y << ")\n";
}
