#include "MenuState.h"

MenuState::MenuState(APP * a_app) : IGameState(a_app) {}

MenuState::~MenuState()
{
}

void MenuState::Update(float deltaTime)
{
}

void MenuState::Draw()
{
#ifdef _DEBUG
	std::cout << "Menu - Draw" << std::endl;
#endif // _DEBUG
}
