#pragma once
#include <Texture.h>
#include <memory>
#include "ResourceManager.h"

class Player
{
public:
	Player(TexturePtr texture) : m_ship(texture) {};
	~Player() {};
	
private:
	TexturePtr m_ship;
};

