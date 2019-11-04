#include "PhysicsAndFixedTimeStepApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsAndFixedTimeStepApp();

	// initialise and loop
	app->run("ARK: Physics & Fixed Time Step", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}