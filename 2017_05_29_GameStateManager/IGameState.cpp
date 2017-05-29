#include "IGameState.h"

#pragma region Constructors
IGameState::IGameState(APP* a_app) : m_app(a_app) {}

IGameState::~IGameState()
{
}
#pragma endregion

void IGameState::Update(float deltaTime)
{
}

void IGameState::Draw()
{
}
