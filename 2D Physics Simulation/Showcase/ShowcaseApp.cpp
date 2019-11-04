#include "ShowcaseApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include <Gizmos.h>
#include <Plane.h>
#include <AABB.h>
#include <Sphere.h>
#include <Polygon.h>

ShowcaseApp::ShowcaseApp() {

}

ShowcaseApp::~ShowcaseApp() {

}

bool ShowcaseApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity({ 0, -75 });
	m_physicsScene->setTimeStep(0.01f);

#pragma region Setup Scene

	// boundaries
	Plane* bottom = new Plane({ 0, 1 }, -55);
	Plane* left =	new Plane({ 1, 0 }, -98);
	Plane* top =	new Plane({ 0,-1 }, -55);
	Plane* right =	new Plane({-1, 0 }, -98);

	Plane* bLeft =	new Plane({ .707f, .707f }, -85);
	Plane* tLeft =	new Plane({ .707f,-.707f }, -85);
	Plane* bRight =	new Plane({-.707f, .707f }, -85);
	Plane* tRight = new Plane({-.707f,-.707f }, -85);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(right);

	m_physicsScene->addActor(bLeft);
	m_physicsScene->addActor(tLeft);
	m_physicsScene->addActor(bRight);
	m_physicsScene->addActor(tRight);

	// obstacles
	Polygon* hexagon = new Polygon({ {-11, 0}, {-5, 10}, {5, 10}, {11, 0}, {5, -10}, {-5, -10} });
	hexagon->setStatic(true);

	AABB* leftPlat = new AABB({ -50, 0 }, { 0, 0 }, { 15, 2.5f }, 1);
	leftPlat->setStatic(true);

	AABB* rightPlat = new AABB({ 50, 0 }, { 0, 0 }, { 15, 2.5f }, 1);
	rightPlat->setStatic(true);

	m_physicsScene->addActor(hexagon);
	m_physicsScene->addActor(leftPlat);
	m_physicsScene->addActor(rightPlat);

#pragma endregion

	// spheres
	for (int i = 0; i < 15; i++) {
		float x = -60 + (8 * i);
		Sphere* s = new Sphere({ x, 40 }, { 10, 0 }, 2, 1, 0, 0, 0, 0, 1, {1, 0, 0, 1});
		m_physicsScene->addActor(s);
	}

	// AABBs
	for (int i = 0; i < 5; i++) {
		float x = -30 + (15 * i);
		AABB* a = new AABB({ x, -40 }, { -10, 0 }, { 2.5f, 2.5f }, 1, 0, 0, 1, { 0, 1, 0, 1 });
		m_physicsScene->addActor(a);
	}

	// Polygons
	Polygon* tri0 = new Polygon({ {-3, -3}, {-3, 3}, {3, -3} }, { -80, 0 }, { 0, 0 }, { 0, 0, 1, 1 }, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(tri0);

	Polygon* tri1 = new Polygon({ {-3, -3}, {3, 3}, {3, -3} }, { 80, 0 }, { 0, 0 }, { 0, 0, 1, 1 }, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(tri1);

	Polygon* tri2 = new Polygon({ {-3, -3}, {0, 3}, {3, -3} }, { 0, 20 }, { 0, 0 }, { 0, 0, 1, 1 }, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(tri2);

	Polygon* pent0 = new Polygon({ {-3, .75f}, {0, 3}, {3, .75f}, {2, -3}, {-2, -3} }, { -80, 30 }, { 0, 0 }, { 1, 1, 0, 1 }, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(pent0);

	Polygon* pent1 = new Polygon({ {-3, .75f}, {0, 3}, {3, .75f}, {2, -3}, {-2, -3} }, { 80, 30 }, { 0, 0 }, { 1, 1, 0, 1 }, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(pent1);

	Polygon* oct0 = new Polygon({ {-4, -1.75f}, {-4, 1.75f}, {-1.75, 4}, {1.75, 4}, {4, 1.75f}, {4, -1.75f}, {1.75f, -4}, {-1.75f, -4} }, { 0, -20 }, { 0, 0 }, {0, 1, 1, 1}, 0, 0, 1, 0, 0);
	m_physicsScene->addActor(oct0);

	return true;
}

void ShowcaseApp::shutdown() {

	delete	m_font;
	delete	m_2dRenderer;
	delete	m_physicsScene;
	
	aie::Gizmos::destroy();
}

void ShowcaseApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	// update physics scene
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void ShowcaseApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1, 1));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}