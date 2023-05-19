/*
 * This file is a part of Kek3D. 
 * 
 * Author: Kevin Sadi
 */

#include "include/engine/Engine.hpp"

using namespace kek3d;

/*
 * The main entrypoint for Kek3d
 * 
 * Creates window, loads textures, loads vertices, has main render loop
 */
int main() {

	
	Engine* engine = new Engine();
	// loops until render loop ends
	engine->Start();
	delete engine;

	return 0;
}
