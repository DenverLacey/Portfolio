#pragma once
#include <glm/ext.hpp>

class CollisionInfo
{
public:
	CollisionInfo();
	~CollisionInfo();

	CollisionInfo(const glm::vec2& normal, const glm::vec2& contact);

	bool operator != (const CollisionInfo& other);

	glm::vec2 normal;
	glm::vec2 contact;

	const static CollisionInfo zero;
};

