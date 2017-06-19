#include "MenuState.h"
#include "_2017_05_29_GameStateManagerApp.h"

MenuState::MenuState(APP * a_app) : IGameState(a_app) {
	stateDrawText = "Menu - Draw";
}

MenuState::~MenuState()
{
}

