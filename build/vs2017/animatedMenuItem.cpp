#include "animatedMenuItem.h"



MenuItem::MenuItem(gef::Platform &platform, const char *fileName, float x, float y, float u, float v, float width, float height)
{
	//Because this is a 2D animated sprite, we have values for the u and v values,
	//that will be offset each frame.
	uPos = 0;
	yPos = 0;

	pngLoader.Load(fileName, platform, imageData);
	texture = gef::Texture::Create(platform, imageData);

	sprite.set_width(width);
	sprite.set_height(height);
	sprite.set_position(x, y, 1);
	sprite.set_uv_height(u);
	sprite.set_uv_width(v);
	sprite.set_uv_position(gef::Vector2(uPos, yPos));
	sprite.set_texture(texture);
}


MenuItem::~MenuItem()
{
}

void MenuItem::update(float time)
{
	//Due to the rate of change we want, the value time gives us is
	//far too large. A reduction value is used.
	uPos += time / ANIMATION_SPEED_REDUCTION;
	yPos += time / ANIMATION_SPEED_REDUCTION;

	sprite.set_uv_position(gef::Vector2(uPos, yPos));
	sprite.set_texture(texture);

	//Worth noting this version doesn't support sprite sheets,just drifting effect in
	//uv coordiates
}