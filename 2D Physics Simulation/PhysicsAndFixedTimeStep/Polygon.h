#pragma once
#include "Rigidbody.h"
#include <vector>

class Polygon : public Rigidbody
{
public:
	Polygon(const std::vector<glm::vec2>& vertices,
			const glm::vec2& position = glm::vec2(0, 0), const glm::vec2& velocity = glm::vec2(0, 0), const glm::vec4& colour = glm::vec4(1, 1, 1, 1),
			float rotation = 0.f, float angularVelocity = 0.f, float mass = 1.f,
			float drag = 0.3f, float angularDrag = 0.3f, float elasticity = 1.f);
	~Polygon();

	void fixedUpdate(const glm::vec2& gravity, float timeStep) override;
	void makeGizmo() const override;
	void resetPosition() override;

	const std::vector<glm::vec2>& getVertices() const;

	void debug() const override;

protected:
	std::vector<glm::vec2> m_relativeVertices;
	std::vector<glm::vec2> m_vertices;
};

