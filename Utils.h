#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const int MAX_RATING = 100;
const int MAX_TARGETS = 5;
const int S_TIER_SCORE = MAX_TARGETS * MAX_RATING;
const int S_TIER_SPEED = MAX_TARGETS * 0.45 * 1000;

struct ColorOption {
	int r;
	int g;
	int b;
};