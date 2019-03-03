#pragma once
#include "Menu.h"
class StaticMenuItem : public Menu
{
public:
	StaticMenuItem(gef::Platform &platform, const char *fileName, float x, float y, float u, float v, float width, float height);
	~StaticMenuItem();


private:
	//To allow for a highlight effect on the items
	bool isSelected;
	gef::Sprite backing;
};

