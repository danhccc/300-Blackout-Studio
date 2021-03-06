#include "Game.h"
#include "Bomb.h"

Bomb::Bomb()
{
    TheTextureManager::Instance()->load("../Assets/textures/bomb.png", "bomb", TheGame::Instance()->getRenderer());
    setPosition(glm::vec2(0, 0));

    glm::vec2 size = TheTextureManager::Instance()->getTextureSize("bomb");
    setWidth(size.x);
    setHeight(size.y);
    setIsColliding(false);
    setType(GameObjectType::BOMB);
}

void Bomb::draw()
{
    if (isSet)
        TheTextureManager::Instance()->draw("bomb", getPosition().x, getPosition().y, TheGame::Instance()->getRenderer(), false);
}

void Bomb::update()
{
    if (getBomb())
    {
        if (bombTimer < bombTimerMax)
        {
            bombTimer++;
        }
        else
        {
            bombTimer = 0;
			setExplode(true);
            setBomb(false);
        }
    }
}

void Bomb::clean()
{
}

void Bomb::setBomb(bool set)
{
    isSet = set;
}

bool Bomb::getBomb()
{
    return isSet;
}

void Bomb::setExplode(bool expl)
{
	hasExploded = expl;
}

bool Bomb::getExplode()
{
	return hasExploded;
}