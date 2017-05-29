#include "PlayState.h"
#include "_2017_05_29_GameStateManagerApp.h"

PlayState::PlayState(APP* a_app) : IGameState(a_app) {}

PlayState::~PlayState()
{
}

void PlayState::Update(float deltaTime)
{
}

void PlayState::Draw()
{
#ifdef _DEBUG
	std::cout << "Play - Draw" << std::endl;
#endif // DEBUG

	m_app->GetRenderer()->begin();

	m_app->GetRenderer()->drawBox(100, 100, 100, 100);

	m_app->GetRenderer()->end();
}
