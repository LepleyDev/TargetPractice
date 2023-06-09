#include "ColorPicker.h"

void ColorPicker::Init(int r, int g, int b, int x, int y) {
	posRect = { x, y, 20, 20 };

	red = r;
	green = g;
	blue = b;
}

bool ColorPicker::IsHit(int mouseX, int mouseY) {
	return (
		mouseX >= posRect.x &&
		mouseX <= posRect.x + posRect.w &&
		mouseY >= posRect.y &&
		mouseY <= posRect.y + posRect.h
		);
}

SDL_Rect* ColorPicker::GetRect() {
	return &posRect;
}