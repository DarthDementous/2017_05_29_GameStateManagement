#include "IGameState.h"
#include "_2017_05_29_GameStateManagerApp.h"

#pragma region Constructors
IGameState::IGameState(APP* a_app) : m_app(a_app) {}

IGameState::~IGameState()
{
}
#pragma endregion

void IGameState::Update(float deltaTime)
{
#ifdef _DEBUG
	xPosText += 500 * deltaTime;
	if (xPosText > (m_app->getWindowWidth() - DEFAULT_TEXT_POS * 8)) {
		xPosText = DEFAULT_TEXT_POS;
	}
#endif
}

void IGameState::Draw()
{
#ifdef _DEBUG
	m_app->GetRenderer()->drawText(m_app->m_font, stateDrawText, xPosText, (float)(m_app->getWindowHeight() - 25));
#endif // _DEBUG
}
