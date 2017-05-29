#pragma once

#include "IGameState.h"

class PauseState : public IGameState {
public:
	/*
	*	@brief	State dictating how the application pauses.
	*	@param	a_app is the application we pass in to the base class's constructor so we have access to its members and functions.
	*/
	PauseState(APP* a_app);
	virtual ~PauseState();

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
private:
};
