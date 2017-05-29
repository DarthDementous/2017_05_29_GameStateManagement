#pragma once
#include <iostream>

#define APP _2017_05_29_GameStateManagerApp

// Forward declarations
class APP;

class IGameState {
public:
	/*
	*	@brief	Base class that defines states of operation for an application.
	*	@param	a_app is the application we pass in so we have access to its members and functions.
	*/
	IGameState(APP* a_app);
	virtual ~IGameState();						// Base classes that are inherited from must have virtual destructor so memory clean up happens for base class as well

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	APP*					m_app;
private:
};