#include "PhysicsAndFixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

#include <iostream>

#include "PhysicsScene.h"
#include "Sphere.h"

PhysicsAndFixedTimeStepApp::PhysicsAndFixedTimeStepApp() {

}

PhysicsAndFixedTimeStepApp::~PhysicsAndFixedTimeStepApp() {

}

bool PhysicsAndFixedTimeStepApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -1.f));
	m_physicsScene->setTimeStep(0.01f);
	
	auto* ball = new Sphere(glm::vec2(-40, -10), glm::vec2(0, 0), 9.f, 3.f, 0, 0, 0, 0, 1, glm::vec4(0, 0, 1, 1));
	auto* ball1 = new Sphere(glm::vec2(40, 10), glm::vec2(0, 0), 3.f, 1.f, 0, 0, 0, 0, 1, glm::vec4(1, 1, 1, 1));

	m_physicsScene->addActor(ball);
	m_physicsScene->addActor(ball1);

	ball->applyForce(glm::vec2(10, 2.5f));
	ball1->applyForce(glm::vec2(-10, -2.5f));

	m_rocket = new Sphere(glm::vec2(40, -30), glm::vec2(0, 0), 6.f, 5.f, 0, 0, 0, 0, 1, glm::vec4(1, 1, 1, .5f));
	m_physicsScene->addActor(m_rocket);

	m_rocket->applyForce(glm::vec2(0, 20));

	return true;
}

void PhysicsAndFixedTimeStepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;

	delete m_physicsScene;

	for (glm::vec2* v : m_fuel) {
		if (v != nullptr) {
			delete v;
			v = nullptr;
		}
	}
}

void PhysicsAndFixedTimeStepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	static float M = 0.5f;

	m_rocket->setMass(m_rocket->getMass() - M * deltaTime);
	
	if (m_rocket->getMass() <= 1.f) {
		m_rocket->setMass(1.f);
	}
	else {
		float y = m_rocket->getPosition().y - m_rocket->getRadius();
		auto* fuel = new glm::vec2(m_rocket->getPosition().x, y);
		m_fuel.push_back(fuel);
		m_rocket->applyForce(glm::vec2(0, m_rocket->getMass() * 1.2f * deltaTime));
	}

	for (glm::vec2* v : m_fuel) {
		v->x += m_physicsScene->getGravity().x;
		v->y += m_physicsScene->getGravity().y;
		aie::Gizmos::add2DCircle(*v, M, 32, glm::vec4(1, 1, 0, 1));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsAndFixedTimeStepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}