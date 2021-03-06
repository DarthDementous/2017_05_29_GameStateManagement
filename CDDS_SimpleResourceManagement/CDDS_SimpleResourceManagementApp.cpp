#include "CDDS_SimpleResourceManagementApp.h"
#include "Font.h"
#include "Input.h"
#include "Player.h"
#include <iostream>

CDDS_SimpleResourceManagementApp::CDDS_SimpleResourceManagementApp() {

}

CDDS_SimpleResourceManagementApp::~CDDS_SimpleResourceManagementApp() {

}

bool CDDS_SimpleResourceManagementApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_images.get("./textures/grass.png");
	m_images.get("./textures/car.png");

	TexturePtr pShip = m_images.get("./textures/ship.png");
	m_player = new Player(pShip);

	std::cout << "images loaded: " << m_images.getCount() << std::endl;

	return true;
}

void CDDS_SimpleResourceManagementApp::shutdown() {

	std::cout << "images loaded: " << m_images.getCount() << std::endl;
	
	delete m_player;

	m_images.collectGarbage();

	std::cout << "images loaded: " << m_images.getCount() << std::endl;

	delete m_font;
	delete m_2dRenderer;
}

void CDDS_SimpleResourceManagementApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void CDDS_SimpleResourceManagementApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!

	// output some text
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}
