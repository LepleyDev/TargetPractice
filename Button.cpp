#include "Button.h"

void Button::Init(SDL_Texture* t, int x, int y, int w, int h) {
	texture = t;

	posRect.x = x;
	posRect.y = y;
	posRect.w = w;
	posRect.h = h;
}

bool Button::IsHit(int mouseX, int mouseY) {
	return (
		mouseX >= posRect.x &&
		mouseX <= posRect.x + posRect.w &&
		mouseY >= posRect.y &&
		mouseY <= posRect.y + posRect.h
	);
}

SDL_Rect* Button::GetPos() {
	return &posRect;
}