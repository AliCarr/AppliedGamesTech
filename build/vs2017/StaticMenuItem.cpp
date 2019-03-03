#include "StaticMenuItem.h"



StaticMenuItem::StaticMenuItem(gef::Platform &platform, const char *fileName, float x, float y, float u, float v, float width, float height)
{
	pngLoader.Load(fileName, platform, imageData);
	texture = gef::Texture::Create(platform, imageData);

	sprite.set_width(width);
	sprite.set_height(height);
	sprite.set_position(x, y, 1);
	sprite.set_uv_height(u);
	sprite.set_uv_width(v);
	sprite.set_uv_position(gef::Vector2(0, 0));
	sprite.set_texture(texture);


	//Load in background highlight effect 
	pngLoader.Load(fileName, platform, imageData);
	texture = gef::Texture::Create(platform, imageData);

	backing.set_width(width + 20);
	backing.set_height(height + 20);
	backing.set_position(x - 10, y - 10, 1);
	backing.set_uv_height(u);
	backing.set_uv_width(v);
	backing.set_uv_position(gef::Vector2(0, 0));
	backing.set_texture(texture);
}


StaticMenuItem::~StaticMenuItem()
{
}
