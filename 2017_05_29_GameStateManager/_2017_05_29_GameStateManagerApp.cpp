#include "_2017_05_29_GameStateManagerApp.h"
#include "GameStateManager.h"
#include "PlayState.h"
#include "PauseState.h"
#include "MenuState.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

_2017_05_29_GameStateManagerApp::_2017_05_29_GameStateManagerApp() {

}

_2017_05_29_GameStateManagerApp::~_2017_05_29_GameStateManagerApp() {

}

bool _2017_05_29_GameStateManagerApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	
#if 1
#pragma region State Manager
	m_stateManager = new GameStateManager;

	m_stateManager->PushState("MENU", new MenuState(this));
	m_stateManager->PushState("PLAY", new PlayState(this));
	m_stateManager->PushState("PAUSE", new PauseState(this));

	m_stateManager->SetState("MENU");
	m_stateManager->SetState("PLAY");
#pragma endregion
#endif
	return true;
}

void _2017_05_29_GameStateManagerApp::shutdown() {
	delete m_stateManager;
	delete m_font;
	delete m_2dRenderer;
}

void _2017_05_29_GameStateManagerApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// Update states
	m_stateManager->Update(deltaTime);
}

void _2017_05_29_GameStateManagerApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// Draw states
	m_stateManager->Draw();

	// done drawing sprites
	m_2dRenderer->end();
}