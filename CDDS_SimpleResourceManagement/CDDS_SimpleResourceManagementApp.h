#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "ResourceManager.h"

class Player;

class CDDS_SimpleResourceManagementApp : public aie::Application {
public:

	CDDS_SimpleResourceManagementApp();
	virtual ~CDDS_SimpleResourceManagementApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	ResourceManager<aie::Texture>	m_images;
	Player*				m_player;
};