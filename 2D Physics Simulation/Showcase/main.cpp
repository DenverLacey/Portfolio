#include "ShowcaseApp.h"
#include <iostream>

int main() {
	
	// log memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// allocation
	auto app = new ShowcaseApp();

	// initialise and loop
	app->run("Denver Lacey 2D Physics Engine", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}