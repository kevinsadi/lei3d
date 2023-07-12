/*
 * This file is a part of lei3d. 
 * 
 * Author: Kevin Sadi
 */

#include "include/engine/Engine.hpp"
#include "include/util/Log.hpp"

using namespace lei3d;

/*
 * The main entrypoint for lei3d
 * 
 * Creates window, loads textures, loads vertices, has main render loop
 */
int main() {
	lei3d::Log::Init();
	LEI_TRACE("Initialized Logger");
	LEI_WARN("This is a Warning");
	LEI_ERROR("This is an error");
	Engine* engine = new Engine();
	// loops until render loop ends
	engine->Start();
	delete engine;

	return 0;
}
