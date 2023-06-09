#pragma once

#include "Utils.h"

class Target
{
public:
	void Init();
	bool IsHit(int x, int y);
	int HitRating(int mouseX, int mouseY);

	SDL_Rect* GetRect();

private:
	SDL_Rect posRect;
};

