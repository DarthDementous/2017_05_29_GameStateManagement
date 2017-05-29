#include "CDDS_SnackFactoryApp.h"

int main() {
	
	auto app = new CDDS_SnackFactoryApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}