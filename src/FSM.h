#pragma once
#ifndef __FSM__
#define __FSM__

#include <vector>
#include <SDL.h>
using namespace std;

enum StateType
{
	PAUSE,
	GAME,
	TITLE,
	NUM_OF_STATES
};

class State // Base state class.
{
protected:
	State() {}
public:
	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void render();
	virtual void exit() = 0;
	virtual void resume();
	StateType getStateType();
	void setType(StateType newType);
	StateType m_stateType;
	SDL_Event event;
};

class PauseState : public State
{
public:
	PauseState();
	void enter();
	void update();
	void render();
	void exit();
};

class GameState : public State
{
public:
	GameState();
	void enter();
	void update();
	void render();
	void exit();
	void resume();
};

class TitleState : public State
{
public:
	TitleState();
	void enter();
	void update();
	void render();
	void exit();
};

class FSM
{
private:
	vector<State*> m_vStates;
public: 
	FSM();
	~FSM();
	void update();
	void render();
	void changeState(State* pState); // Normal state change.
	void pushState(State* pState); // GameState to PauseState.
	void popState(); // PauseState to GameState.
	void clean();
	vector<State*>& getStates();
};

#endif /* defined __FSM__ */