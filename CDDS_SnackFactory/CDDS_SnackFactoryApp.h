#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "IPrototype.h"
#include <list>


class Snack;
class SnackFactory;

class CDDS_SnackFactoryApp : public aie::Application {
public:

	CDDS_SnackFactoryApp();
	virtual ~CDDS_SnackFactoryApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	std::unique_ptr<SnackFactory> m_snackFactory;
	std::list<std::shared_ptr<Snack>> m_snacks;
};