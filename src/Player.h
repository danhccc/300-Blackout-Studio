#pragma once
#ifndef __Player__
#define __Player__

#include "GameObject.h"
#include "TextureManager.h"
#include "SoundManager.h"

class Player : public GameObject {
public:
	Player();
	~Player();
	
	// Draw the object
	void draw();

	// Update the object
	void update();

	// remove anything that needs to be deleted
	void clean();

    bool getRequest();
    void setRequest(bool request);
	const int m_iPlayerSpeed = 4;

	
	
private:
	//bool m_bKeyDown; // True if a key is currently pressed down, otherwise false !!Might not need
    bool requestBomb = false;
	
};


#endif /* defined (__Player__) */