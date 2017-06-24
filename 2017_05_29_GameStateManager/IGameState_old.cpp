#include "IGameState.h"
#include "_2017_05_29_GameStateManagerApp.h"
#include <Application.h>

#pragma region Constructors
IGameState::IGameState(aie::Application* a_app) : m_app(a_app) {}

IGameState::~IGameState()
{
}
#pragma endregion
