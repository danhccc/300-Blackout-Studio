#pragma once
#ifndef __Game__
#define __Game__

// Core Libraries
#include <algorithm>
#include <iostream>
#include <vector>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Game Managers
#include "TextureManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"

// Game Objects
#include "Player.h"
#include "Map.h"
#include "Wall.h"
#include "Bomb.h"
#include "FSM.h"
#include "Explosion.h"
#include "Enemy.h"

const int FPS = 60;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 768;
const int DELAY_TIME = 1000.0f / FPS;


class Game
{


public:

	// See if a key is pressed down
	bool checkForKeystroke(SDL_Scancode c);
	
	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}

		return s_pInstance;
	}

	
	
	// simply set the running variable to true
	void init() { m_bRunning = true; }

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	// public functions
	void render();
	void update();
	void handleEvents();
	void clean();

	// a function to access the private running variable
	bool running() { return m_bRunning; }


	// getters
	SDL_Renderer* getRenderer();
	glm::vec2 getMousePosition();
	FSM& GetFSM();

	// int* m_pCurrentMap[12][20];

	void createGameObjects();

	// Returns mousebutton presses
	bool GetMouseBtn(int i);

private:
	Game();
	~Game();

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	FSM* m_pFSM; // Pointer to a state machine instance.
	
	int m_currentFrame;

	bool m_bRunning;
	
	// Bool array for mousebuttons (0 = Left, 1 = Right)
	bool m_bMouseBtn[2] = { false, false };
	
	static Game* s_pInstance;

	// GameObjects
	Player* m_pPlayer;
    Bomb* m_pBomb; // May need to make a vector later
	Explosion* m_pExplosion; // May need to make a vector later
	Enemy* m_pEnemy; // Will need to make a vector later

    // Vector of Wall GameObjects
    std::vector<Wall*> m_pWallVec;

	
	
	const Uint8* m_iKeystates; // Keyboard state container.
	
	glm::vec2 m_mousePosition; // Not currently used, will be used when a main menu is added
};

typedef Game TheGame;

#endif  defined (__Game__) 