#pragma once

#include "Application.h"
#include "Renderer2D.h"

class GameStateManager;

class _2017_05_29_GameStateManagerApp : public aie::Application {
public:

	_2017_05_29_GameStateManagerApp();
	virtual ~_2017_05_29_GameStateManagerApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	aie::Renderer2D*	GetRenderer() { return m_2dRenderer; }
	GameStateManager*	GetStateManager() { return m_stateManager; }

protected:
	GameStateManager*	m_stateManager = nullptr;

	aie::Renderer2D*	m_2dRenderer = nullptr;

public:
	aie::Font*			m_font = nullptr;
};