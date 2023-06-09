#pragma once

#include <map>
#include "Utils.h"

class ColorPicker
{
public:
	void Init(int r, int g, int b, int x, int y);
	bool IsHit(int mouseX, int mouseY);

	SDL_Rect* GetRect();

	int red, green, blue;

private:
	SDL_Rect posRect;
};

