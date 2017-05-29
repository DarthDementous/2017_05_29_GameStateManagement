#include "CDDS_SimpleResourceManagementApp.h"

int main() {
	
	auto app = new CDDS_SimpleResourceManagementApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}