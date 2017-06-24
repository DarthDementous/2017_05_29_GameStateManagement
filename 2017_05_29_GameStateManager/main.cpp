#include "_2017_05_29_GameStateManagerApp.h"
#include <catch.hpp>

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto app = new _2017_05_29_GameStateManagerApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}