#include "PauseState.h"

PauseState::PauseState(APP * a_app) : IGameState(a_app) {}

PauseState::~PauseState()
{
}

void PauseState::Update(float deltaTime)
{
}

void PauseState::Draw()
{
#ifdef _DEBUG
	std::cout << "Pause - Draw" << std::endl;
#endif // _DEBUG
}
