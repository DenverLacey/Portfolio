#include "CollisionInfo.h"


const CollisionInfo CollisionInfo::zero = { glm::vec2(0, 0), glm::vec2(0, 0) };

CollisionInfo::CollisionInfo()
	: normal({0, 0}), contact({0, 0})
{}


CollisionInfo::~CollisionInfo()
{
}

CollisionInfo::CollisionInfo(const glm::vec2& normal, const glm::vec2& contact)
	: normal(normal), contact(contact)
{}

bool CollisionInfo::operator!=(const CollisionInfo& other)
{
	if (normal != other.normal || contact != other.contact)
		return true;
	else
		return false;
}
