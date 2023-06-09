#include "Target.h"

void Target::Init() {
	posRect.x = rand() % (SCREEN_WIDTH - 50);
	posRect.y = rand() % (SCREEN_HEIGHT - 100) + 50;
	posRect.w = 50;
	posRect.h = 50;
}

bool Target::IsHit(int mouseX, int mouseY) {
	return (
		mouseX >= posRect.x &&
		mouseX <= posRect.x + posRect.w &&
		mouseY >= posRect.y &&
		mouseY <= posRect.y + posRect.h
	);
}

int Target::HitRating(int mouseX, int mouseY) {
	int centerX = posRect.x + (posRect.w / 2);
	int centerY = posRect.y + (posRect.h / 2);
	int missX = abs(mouseX - centerX);
	int missY = abs(mouseY - centerY);
	int missDistance = missX + missY;

	if (missDistance <= 4) {
		return MAX_RATING;
	}
	else {
		return MAX_RATING - missDistance;
	}
}

SDL_Rect* Target::GetRect() {
	return &posRect;
}