#include "Game.h"

Game* Game::s_pInstance = 0;

SDL_Renderer* Game::getRenderer()
{
	return m_pRenderer;
}

glm::vec2 Game::getMousePosition()
{
	return m_mousePosition;
}

SDL_Renderer* getRenderer();

bool Game::checkForKeystroke(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

Game::Game()
{
	m_iKeystates = SDL_GetKeyboardState(nullptr); // start reading keyboard states, returns pointer
	srand(time(NULL)); // Initialize randomness for entire game, currently not used with the mapping build
}

Game::~Game()
{
}

void Game::createGameObjects()
{
	m_pPlayer = new Player();
	m_pBomb = new Bomb();
	m_pExplosion = new Explosion();
	m_pEnemy = new Enemy();
	
    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        for(int x = 0; x < MAP_WIDTH; x++)
        {
            if (demoLevel[y][x] == 1)
            {
                Wall* newWall = new Wall(x,y);
                newWall->setPosition(glm::vec2(64 * x, 64 * y));
                m_pWallVec.push_back(newWall);
            }
        }
    }
}



bool Game::init(const char* title, int xpos, int ypos, int height, int width, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		std::cout << "SDL Init success" << std::endl;

		// if succeeded create our window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);

		// if window creation successful create our renderer
		if (m_pWindow != 0)
		{
			std::cout << "window creation success" << std::endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success" << std::endl;
				
			}
			else
			{
				std::cout << "renderer init failure" << std::endl;
				return false; // render int fail
			} 

			createGameObjects();
		}
		else
		{
			std::cout << "window init failure" << std::endl;
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init failure" << std::endl;
		return false; //SDL could not intialize
	}
	m_pFSM = new FSM();
	m_pFSM->changeState(new TitleState());
	std::cout << "init success" << std::endl;
	m_bRunning = true; // everything initialized successfully - start the main loop
	

	return true;
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	
	switch (GetFSM().getStates().back()->getStateType())
	{
	case TITLE:
		m_pFSM->render();
		break;
	case GAME:
		{
			m_pFSM->render();
			if (m_pBomb->getBomb())
			{
				m_pBomb->draw();
			}
				
			m_pPlayer->draw();
			m_pEnemy->draw();

			for (auto& currentWall : m_pWallVec)
			{
				currentWall->draw();
			}

			if (m_pExplosion->getExplosion())
			{
				m_pExplosion->draw();
			}
		}
		break;
	case PAUSE:
		Instance()->GetFSM().getStates().front()->render(); // Invoke Render of GameState.

		if (m_pBomb->getBomb())
		{
			m_pBomb->draw();
		}

		m_pPlayer->draw();
		m_pEnemy->draw();

		for (auto& currentWall : m_pWallVec)
		{
			currentWall->draw();
		}

		if (m_pExplosion->getExplosion())
		{
			m_pExplosion->draw();
		}
		
		m_pFSM->render();
		break;
	default:
		break;
	}

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::update()
{
	// Current order of updates: FSM >> Player >> Bomb >> Explosion >> Enemy >> Wall
	

	switch (GetFSM().getStates().back()->getStateType())
	{
	case TITLE:
		m_pFSM->update();
		break;
	case GAME:
		{
			m_pFSM->update();
			m_pPlayer->update();
			if (m_pPlayer->getRequest() && !m_pBomb->getBomb())
			{
				m_pBomb->setPosition(glm::vec2(m_pPlayer->getPosition()));
				m_pBomb->setBomb(true);
			}
			m_pPlayer->setRequest(false);
			m_pBomb->update();

			// If bomb has exploded...
			if (m_pBomb->getExplode() == true)
			{
				m_pBomb->setExplode(false);

				// ...an explosion happens
				m_pExplosion->setPosition(glm::vec2(m_pBomb->getPosition().x - (m_pBomb->getWidth() * 0.5), m_pBomb->getPosition().y - (m_pBomb->getHeight() * 0.5)));
				m_pExplosion->setExplosion(true);
			}

			if (m_pExplosion->getExplosion())
			{
				// Explosion vs Wall
				for (auto& currentWall : m_pWallVec)
				{
					Collision::basicCollisionCheck(m_pExplosion, currentWall);
					currentWall->update();
					if (currentWall->getDestruction())
					{
						demoLevel[currentWall->getY()][currentWall->getX()] = 0;
						delete currentWall;
						currentWall = nullptr;
					}
				}

				Collision::basicCollisionCheck(m_pExplosion, m_pPlayer);
			}
			m_pExplosion->update();

			m_pEnemy->update();
			
			// Remove any destroyed walls from vector
			if (!m_pWallVec.empty())
				m_pWallVec.erase(remove(m_pWallVec.begin(), m_pWallVec.end(), nullptr), m_pWallVec.end());

			// Player vs Wall collision checker (map based)
			m_pPlayer->setIsColliding(Collision::tileCollisionCheck(m_pPlayer, demoLevel));
			if (m_pPlayer->getIsColliding())
			{
				m_pPlayer->setIsColliding(Collision::playerCollisionMovement(m_pPlayer, demoLevel));
			}


			
		}
		break;
	case PAUSE:
		m_pFSM->update();
		break;
	default:
		break;
	}
	
	
	
}

void Game::clean()
{
	std::cout << "cleaning game" << std::endl;

	// Clean statemachine
	m_pFSM->clean();
	delete m_pFSM;
	//m_pFSM = nullptr; // Optional.

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				m_bMouseBtn[0] = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				// These commented lines are to determine the current mouse position
				//std::cout << "x: " << getMousePosition().x << std::endl;
				//std::cout << "y: " << getMousePosition().y << std::endl;
				m_bMouseBtn[1] = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				m_bMouseBtn[0] = false;
			if (event.button.button == SDL_BUTTON_RIGHT)
				m_bMouseBtn[1] = false;
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		default:
			break;
		}
	}
}


FSM& Game::GetFSM()
{
	return *m_pFSM;
}

bool Game::GetMouseBtn(int i)
{
	return m_bMouseBtn[i];
}