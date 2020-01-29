#pragma once
#ifndef __Enemy__
#define __Enemy__

#include "GameObject.h"
#include "TextureManager.h"

class Enemy : public GameObject {
public:
	Enemy();
	~Enemy();

	// Draw the object
	void draw();

	// Update the object
	void update();

	// remove anything that needs to be deleted
	void clean();

private:

};

#endif /* defined (__Enemy__) */