#pragma once
#include <iostream>

#define APP _2017_05_29_GameStateManagerApp
#define DEFAULT_TEXT_POS 25

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

	void SetUpdateActive(bool a_val) { updateIsActive = a_val; }
	void SetDrawActive(bool a_val) { drawIsActive = a_val; }

	bool IsUpdateActive() { return updateIsActive; }
	bool IsDrawActive() { return drawIsActive; }

protected:
	APP*					m_app;
	
	float xPosText = DEFAULT_TEXT_POS;
	const char* stateDrawText;

	bool updateIsActive = true;
	bool drawIsActive = true;
private:
};