#pragma once

#include "IGameState.h"

class PlayState : public IGameState {
public:
	/*
	*	@brief	State dictating the preview of the game (before menu).
	*	@param	a_app is the application we pass in to the base class's constructor so we have access to its members and functions.
	*/
	PlayState(APP* a_app);
	virtual ~PlayState();				

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
private:
};