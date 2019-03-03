#pragma once

#include "graphics\sprite.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <assets/png_loader.h>
#include "graphics\image_data.h"

namespace gef
{
	class Platform;
	class ImageData;
	class Texture;
	class Sprite;
	class SpriteRenderer;
}

class Menu
{
public:
	Menu();
	~Menu();

	void onRender(gef::SpriteRenderer*);
	virtual void update(float);

protected:
	//Set variables that both static and animated will need
	float x, y, u, v, width, height;
	gef::ImageData imageData;
	gef::Texture *texture;
	gef::Sprite sprite;
	gef::PNGLoader pngLoader;
};

