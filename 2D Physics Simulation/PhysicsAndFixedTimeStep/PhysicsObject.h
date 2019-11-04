#pragma once
#include <glm\ext.hpp>
#include "PhysicsMaterial.h"

const enum class ShapeType {
	PLANE = 0,
	SPHERE,
	POLY,
	
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID), m_physicsMaterial(0.f, 0.f) {};

public:
	virtual ~PhysicsObject() {}
	virtual void fixedUpdate(const glm::vec2& gravity, float timeStep) = 0;
	virtual void debug() const = 0;
	virtual void makeGizmo() const = 0;
	virtual void resetPosition() {}

	ShapeType getShapeID() const { return m_shapeID;  }

	const PhysicsMaterial& getPhysicsMaterial() const { return m_physicsMaterial; }

	void setPhysicsMaterial(const float staticCoefficient, const float kineticCoefficient) {
		m_physicsMaterial = PhysicsMaterial(staticCoefficient, kineticCoefficient);
	}

	void establishContact(const PhysicsMaterial* other, const glm::vec2& contactNormal) {
		m_physicsMaterial.establishContact(other, contactNormal);
	}

	void diestablishContact() {
		m_physicsMaterial.disestablishContact();
	}

protected:
	ShapeType m_shapeID;
	PhysicsMaterial m_physicsMaterial;
};

