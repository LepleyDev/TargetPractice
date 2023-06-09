#pragma once

#include "Utils.h"

class Button
{
public:
	void Init(SDL_Texture* t, int x, int y, int w, int h);
	bool IsHit(int mouseX, int mouseY);

	SDL_Rect* GetPos();

private:
	SDL_Rect posRect;
	SDL_Texture* texture;
};

