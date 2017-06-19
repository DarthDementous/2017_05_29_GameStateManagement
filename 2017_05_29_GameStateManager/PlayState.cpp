#include "PlayState.h"
#include "_2017_05_29_GameStateManagerApp.h"
#include <Input.h>
#include "GameStateManager.h"

PlayState::PlayState(APP* a_app) : IGameState(a_app) {
	stateDrawText = "Play - Draw";
}

PlayState::~PlayState()
{
}

void PlayState::Update(float deltaTime)
{
	// Call super-class update
	IGameState::Update(deltaTime);

	aie::Input *input = aie::Input::getInstance();
	// Pause game and switch to pause state (wasKeyPressed used so check only spans one frame and doesn't carry over into next update loop)
	if (input->wasKeyPressed(aie::INPUT_KEY_P)) {
		m_app->GetStateManager()->PauseStates();
		m_app->GetStateManager()->SetState("PAUSE");
	}
}


