#pragma once
#include "Menu.h"

#define ANIMATION_SPEED_REDUCTION 5000.f
class MenuItem : public Menu
{
public:
	MenuItem(gef::Platform &platform, const char *fileName, float x, float y, float u, float v, float width, float height);
	~MenuItem();

	void update(float);
private:
	//For offsettng the UV map to give the animated effect
	float uPos, yPos;
};

