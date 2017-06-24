#pragma once
#include <iostream>

// Forward declarations
namespace aie {
	class Application;
}

class IGameState {
public:
	/*
	*	@brief	Base class that defines states of operation for an application.
	*	@param	a_app is the application we pass in so we have access to its members and functions.
	*/
	IGameState(aie::Application* a_app);
	virtual ~IGameState();						// Base classes that are inherited from must have virtual destructor so memory clean up happens for base class as well

	virtual void Initialise() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	void SetUpdateActive(bool a_val) { updateIsActive = a_val; }
	void SetDrawActive(bool a_val) { drawIsActive = a_val; }

	bool IsUpdateActive() { return updateIsActive; }
	bool IsDrawActive() { return drawIsActive; }

protected:
	aie::Application* m_app;

	bool updateIsActive = true;
	bool drawIsActive = true;
private:
};