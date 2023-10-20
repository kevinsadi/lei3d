#include "core/Application.hpp"
#include "logging/Log.hpp"

using namespace lei3d;

/*
 * The main entrypoint for SkyLei the game
 * TODO: game stuffs
 */
int main()
{
	Log::Init();
	LEI_TRACE("Initialized Logger");

	// loops until render loop ends
	Application& app = Application::GetInstance();
	app.Run();

	return 0;
}