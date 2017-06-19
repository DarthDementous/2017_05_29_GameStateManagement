#include "PauseState.h"
#include <Input.h>
#include "_2017_05_29_GameStateManagerApp.h"
#include "GameStateManager.h"

PauseState::PauseState(APP * a_app) : IGameState(a_app) {
	stateDrawText = "Pause - Draw";
}

PauseState::~PauseState()
{
}

void PauseState::Update(float deltaTime)
{
	// Call super-class update
	IGameState::Update(deltaTime);

	aie::Input *input = aie::Input::getInstance();
	// Resume game if it was already paused (wasKeyPressed used so check only spans one frame and doesn't carry over into next update loop)
	if (input->wasKeyPressed(aie::INPUT_KEY_P)) {
		m_app->GetStateManager()->ResumeStates();
		m_app->GetStateManager()->PopState();
	}
}

