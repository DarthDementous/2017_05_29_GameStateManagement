#include "IGameState.h"

#pragma region Constructors
IGameState::IGameState(aie::Application* a_app) : m_app(a_app) {}

IGameState::~IGameState()
{
}
#pragma endregion
