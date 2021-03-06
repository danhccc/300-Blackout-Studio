#include "Player.h"
#include "Game.h"
#include "Bomb.h"

Player::Player()
{
	TheTextureManager::Instance()->load("../Assets/textures/player.png", "player", TheGame::Instance()->getRenderer());
	setPosition(glm::vec2(0, 0));
	setVelocity(glm::vec2(m_iPlayerSpeed, m_iPlayerSpeed));
	

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("player");
	setWidth(size.x);
	setHeight(size.y);
	setIsColliding(false);
	setType(GameObjectType::PLAYER);
}

Player::~Player()
{
}

void Player::draw()
{
	if (getIsActive())
	{
		TheTextureManager::Instance()->draw("player", getPosition().x, getPosition().y, TheGame::Instance()->getRenderer(), false);
	}
}

void Player::update()
{
	if (getIsActive())
	{
		// Set pre-movement position to its previous position
		setPreviousPosition(getPosition());
		// Check if a movement key is pressed, also includes boundary checks
		// y-movement
		if (TheGame::Instance()->checkForKeystroke(SDL_SCANCODE_W))
		{
			setPosition(glm::vec2(getPosition().x, getPosition().y - getVelocity().y));
			if (getPosition().y <= 0) // Top of screen
			{
				setPosition(glm::vec2(getPosition().x, 0));
			}
		}
		if (TheGame::Instance()->checkForKeystroke(SDL_SCANCODE_S))
		{
			setPosition(glm::vec2(getPosition().x, getPosition().y + getVelocity().y));
			if (getPosition().y >= (768 - getHeight())) // Bottom of screen minus player height
			{
				setPosition(glm::vec2(getPosition().x, (768 - getHeight())));
			}
		}
		// x-movement
		if (TheGame::Instance()->checkForKeystroke(SDL_SCANCODE_A))
		{
			setPosition(glm::vec2(getPosition().x - getVelocity().x, getPosition().y));
			if (getPosition().x <= 0) // Left side of screen
			{
				setPosition(glm::vec2(0, getPosition().y));
			}
		}
		if (TheGame::Instance()->checkForKeystroke(SDL_SCANCODE_D))
		{
			setPosition(glm::vec2(getPosition().x + getVelocity().x, getPosition().y));
			if (getPosition().x >= (1280 - getWidth())) // Right side of screen minus player width
			{
				setPosition(glm::vec2((1280 - getWidth()), getPosition().y));
			}
		}
		// Check if key to lay bomb is pressed
		if (TheGame::Instance()->checkForKeystroke(SDL_SCANCODE_SPACE))
		{
			setRequest(true);
		}
	}
}

void Player::clean()
{
}

bool Player::getRequest()
{
    return requestBomb;
}

void Player::setRequest(bool request)
{
    requestBomb = request;
}

