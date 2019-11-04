#pragma once
#include "Polygon.h"
#include <vector>

class AABB : public Polygon
{
public:
	AABB();
	~AABB();

	AABB(const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& extents,
		 float mass, float drag = 0.3f, float angularDrag = 0.3f, 
		 float elasticity = 1.f, const glm::vec4& colour = glm::vec4(1, 1, 1, 1));

public:
	void makeGizmo() const override;

	const glm::vec2 getMin() const;
	const glm::vec2 getMax() const;
	const glm::vec2& getExtents() const;

	void debug() const override;

protected:
	glm::vec2 m_extents;
};

