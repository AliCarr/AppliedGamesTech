#include "Menu.h"

Menu::Menu()
{
}

Menu::~Menu()
{
	delete texture;
	texture = NULL;
}

void Menu::onRender(gef::SpriteRenderer *spriteRenderer)
{
	sprite.set_texture(texture);
	spriteRenderer->DrawSprite(sprite);
}

void Menu::update(float time)
{
	sprite.set_texture(texture);
}
