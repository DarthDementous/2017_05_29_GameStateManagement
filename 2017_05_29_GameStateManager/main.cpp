#include "_2017_05_29_GameStateManagerApp.h"

int main() {
	
	auto app = new _2017_05_29_GameStateManagerApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}