#pragma once

#include "IGameState.h"

class MenuState : public IGameState {
public:
	/*
	*	@brief	State dictating how the menu is displayed.
	*	@param	a_app is the application we pass in to the base class's constructor so we have access to its members and functions.
	*/
	MenuState(APP* a_app);
	virtual ~MenuState();

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
private:
};