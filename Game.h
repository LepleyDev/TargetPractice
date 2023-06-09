#pragma once

#include "Utils.h"
#include "Target.h"
#include "Button.h"
#include "ColorPicker.h"
#include <vector>

class Game
{
public:
	bool Init();

	void LoadTextures();
	void LoadAudio();
	void LoadColors();
	void LoadButtons();
	void LoadTargets();

	void CalculateStats();
	void GameLoop();
	void Shutdown();

	SDL_Texture* LoadTexture(string path);
	SDL_Texture* ConvertTextToTexture(string text, SDL_Rect* targetRect, bool center);

private:
	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning = true;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Rect mousePosRect = { 0, 0, 15, 15 };
	SDL_Rect titlePosRect = { SCREEN_WIDTH / 2 - 150, 50, 300, 80 };
	SDL_Rect scorePosRect = { SCREEN_WIDTH - 150, 20, 100, 50 };
	SDL_Rect timerPosRect = { 50, 20, 300, 50 };
	SDL_Rect scoreTierRect = { 0, 100, 0, 0 };
	SDL_Rect speedTierRect = { 0, 130, 0, 0 };

	int mouseX, mouseY, score;
	int currentGameState;
	int startTime, endTime;
	int timer = 0;	// MS
	int crosshairRed = 0;
	int crosshairGreen = 255;
	int crosshairBlue = 255;

	string speedTier, scoreTier;

	// Colors
	SDL_Rect selectedRect = { SCREEN_WIDTH / 2 - 102, 208, 24, 24 };

	ColorOption colorOptions[8] = {
		{0, 255, 255},		// Teal
		{255, 0, 0},		// Red
		{255, 255, 255},	// White
		{0, 255, 0},		// Green
		{0, 0, 255},		// Blue
		{255, 0, 255},		// Purple
		{255, 255, 0},		// Yellow
		{0, 0, 0}			// Black
	};

	vector<ColorPicker*> colorPickers;
	ColorPicker* currentColorPicker;

	vector<Target*> targets;
	Target* currentTarget;

	Button* startButton;
	Button* quitButton;
	Button* backToMenuButton;

	Mix_Chunk* clickChunk;
	Mix_Chunk* shootChunk;

	SDL_Texture* titleTexture;
	SDL_Texture* backgroundTexture;
	SDL_Texture* crosshairTexture;
	SDL_Texture* targetTexture;
	SDL_Texture* scoreTexture;
	SDL_Texture* timerTexture;
	SDL_Texture* scoreTierTexture;
	SDL_Texture* speedTierTexture;

	SDL_Texture* startButtonTexture;
	SDL_Texture* quitButtonTexture;
	SDL_Texture* backToMenuButtonTexture;

	TTF_Font* textFont;
	SDL_Color textColor = { 255, 255, 255 };
	SDL_Color tealColor = { 0, 255, 255 };

	enum GameState : int {
		Menu,
		Playing,
		Gameover
	};
};

