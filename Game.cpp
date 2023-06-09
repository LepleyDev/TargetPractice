#include "Game.h"

bool Game::Init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		cout << "SDL failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "SDL_Image failed to initialize: " << IMG_GetError() << endl;
		return false;
	}

	if (TTF_Init() != 0)
	{
		cout << "SDL_ttf failed to initialize: " << TTF_GetError() << endl;
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		cout << "SDL_mixer failed to initialize: " << Mix_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow("Target Practice - Speedrun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		cout << "Window failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Renderer failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	SDL_ShowCursor(false);

	// Variables
	score = 0;
	currentGameState = GameState::Menu;
	
	// Font
	textFont = TTF_OpenFont("font.ttf", 22);

	LoadTextures();
	LoadAudio();
	LoadColors();
	LoadButtons();
	LoadTargets();

	return true;
}

void Game::LoadTextures() {
	titleTexture = LoadTexture("title.png");
	backgroundTexture = LoadTexture("background.png");
	crosshairTexture = LoadTexture("crosshair.png");
	targetTexture = LoadTexture("target.png");

	startButtonTexture = LoadTexture("startButton.png");
	quitButtonTexture = LoadTexture("quitButton.png");
	backToMenuButtonTexture = LoadTexture("backToMenuButton.png");
	scoreTexture = ConvertTextToTexture("Score: 0", &scorePosRect, false);
	timerTexture = ConvertTextToTexture("Time: 0.00", &timerPosRect, false);
}

void Game::LoadAudio() {
	clickChunk = Mix_LoadWAV("click-button.wav");
	shootChunk = Mix_LoadWAV("shotgun-firing.wav");

	if (clickChunk == NULL || shootChunk == NULL) {
		cout << "Failed to load WAV: " << Mix_GetError() << endl;
		isRunning = false;
	}
}

void Game::LoadColors() {
	unsigned int i = 0;
	for (auto option : colorOptions) {
		ColorPicker* cp = new ColorPicker;
		cp->Init(option.r, option.g, option.b, SCREEN_WIDTH / 2 - 100 + (i * 26), 210);
		colorPickers.emplace_back(cp);
		++i;
	}

	currentColorPicker = colorPickers.front();
	SDL_SetTextureColorMod(crosshairTexture, currentColorPicker->red, currentColorPicker->green, currentColorPicker->blue);
}

void Game::LoadButtons() {
	startButton = new Button;
	quitButton = new Button;
	backToMenuButton = new Button;

	startButton->Init(startButtonTexture, SCREEN_WIDTH / 2 - 100, 240, 200, 50);
	quitButton->Init(quitButtonTexture, SCREEN_WIDTH / 2 - 100, 300, 200, 50);
	backToMenuButton->Init(backToMenuButtonTexture, SCREEN_WIDTH / 2 - 100, 240, 200, 50);
}

void Game::LoadTargets() {
	score = 0;
	scoreTexture = ConvertTextToTexture("Score: 0", &scorePosRect, false);
	timerTexture = ConvertTextToTexture("Time: 0.00", &timerPosRect, false);

	srand((unsigned)time(NULL));

	for (int i = 0; i < MAX_TARGETS; i++)
	{
		Target* target = new Target;
		target->Init();
		targets.emplace_back(target);
	}

	currentTarget = targets.front();
}

void Game::CalculateStats() {
	// Speed Tier
	if (timer <= S_TIER_SPEED) {
		speedTier = "S";
	}
	else if (timer <= S_TIER_SPEED + 500) {
		speedTier = "A";
	}
	else if (timer <= S_TIER_SPEED + 1000) {
		speedTier = "B";
	}
	else if (timer <= S_TIER_SPEED + 1500) {
		speedTier = "C";
	}
	else if (timer <= S_TIER_SPEED + 2000) {
		speedTier = "D";
	}
	else {
		speedTier = "E";
	}

	// Score Tier
	if (score >= S_TIER_SCORE) {
		scoreTier = "S";
	}
	else if (score >= S_TIER_SCORE - 25) {
		scoreTier = "A";
	}
	else if (score >= S_TIER_SCORE - 50) {
		scoreTier = "B";
	}
	else if (score >= S_TIER_SCORE - 75) {
		scoreTier = "C";
	}
	else if (score >= S_TIER_SCORE - 100) {
		scoreTier = "D";
	}
	else {
		scoreTier = "E";
	}

	scoreTierTexture = ConvertTextToTexture("Score Tier: " + scoreTier, &scoreTierRect, true);
	speedTierTexture = ConvertTextToTexture("Speed Tier: " + speedTier, &speedTierRect, true);
}

void Game::GameLoop() {
	while (isRunning) {
		HandleEvents();
		Update();
		Draw();
	}
}

void Game::HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}

		SDL_GetMouseState(&mouseX, &mouseY);
		mousePosRect.x = mouseX - mousePosRect.w / 2;
		mousePosRect.y = mouseY - mousePosRect.h / 2;

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (currentGameState == GameState::Menu) {
				if (startButton->IsHit(mouseX, mouseY)) {
					Mix_PlayChannel(-1, clickChunk, 0);
					currentGameState = GameState::Playing;
					startTime = SDL_GetTicks();
				}
				else if (quitButton->IsHit(mouseX, mouseY)) {
					Mix_PlayChannel(-1, clickChunk, 0);
					isRunning = false;
				}
				for (int i = 0; i < colorPickers.size(); i++)
				{
					if (colorPickers[i]->IsHit(mouseX, mouseY)) {
						Mix_PlayChannel(-1, clickChunk, 0);
						currentColorPicker = colorPickers[i];
						selectedRect.x = colorPickers[i]->GetRect()->x - 2;
						selectedRect.y = colorPickers[i]->GetRect()->y - 2;
						SDL_SetTextureColorMod(crosshairTexture, colorPickers[i]->red, colorPickers[i]->green, colorPickers[i]->blue);
						break;
					}
				}
			}
			else if (currentGameState == GameState::Gameover) {
				if (backToMenuButton->IsHit(mouseX, mouseY)) {
					Mix_PlayChannel(-1, clickChunk, 0);
					LoadTargets();
					currentGameState = GameState::Menu;
				}
				else if (quitButton->IsHit(mouseX, mouseY)) {
					Mix_PlayChannel(-1, clickChunk, 0);
					isRunning = false;
				}
			}
			else if (currentGameState == GameState::Playing) {
				Mix_PlayChannel(-1, shootChunk, 0);

				if (currentTarget->IsHit(mouseX, mouseY)) {
					score += currentTarget->HitRating(mouseX, mouseY);
					
					targets.erase(targets.begin());

					if (targets.size() > 0) {
						currentTarget = targets.front();
					}
					else {
						currentGameState = GameState::Gameover;
						endTime = timer;
						CalculateStats();
					}
				}
				else {
					score -= 50;
				}

				scoreTexture = ConvertTextToTexture("Score: " + to_string(score), &scorePosRect, false);
			}
		}
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}
}

void Game::Update() {
	if (currentGameState == GameState::Playing) {
		timer = SDL_GetTicks() - startTime;
		stringstream formattedTime;
		int seconds = timer / 1000;
		int milliseconds = timer % 1000;
		formattedTime << "Time: " << (seconds > 0 ? seconds : 0) << "." << milliseconds;
		timerTexture = ConvertTextToTexture(formattedTime.str(), &timerPosRect, false);
	}
}

void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

	if (currentGameState == GameState::Menu) {
		// Draw title
		SDL_RenderCopy(renderer, titleTexture, NULL, &titlePosRect);

		// Draw colors
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &selectedRect);

		for (int i = 0; i < colorPickers.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, colorPickers[i]->red, colorPickers[i]->green, colorPickers[i]->blue, 255);
			SDL_RenderFillRect(renderer, colorPickers[i]->GetRect());
		}

		// Draw buttons
		SDL_RenderCopy(renderer, startButtonTexture, NULL, startButton->GetPos());
		SDL_RenderCopy(renderer, quitButtonTexture, NULL, quitButton->GetPos());
	}
	else if (currentGameState == GameState::Gameover) {
		// Draw buttons
		SDL_RenderCopy(renderer, timerTexture, NULL, &timerPosRect);
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePosRect);
		SDL_RenderCopy(renderer, scoreTierTexture, NULL, &scoreTierRect);
		SDL_RenderCopy(renderer, speedTierTexture, NULL, &speedTierRect);
		SDL_RenderCopy(renderer, backToMenuButtonTexture, NULL, startButton->GetPos());
		SDL_RenderCopy(renderer, quitButtonTexture, NULL, quitButton->GetPos());
	}
	else if (currentGameState == GameState::Playing) {
		// Draw target
		SDL_RenderCopy(renderer, timerTexture, NULL, &timerPosRect);
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePosRect);
		SDL_RenderCopy(renderer, targetTexture, NULL, currentTarget->GetRect());
	}

	// Draw mouse
	SDL_RenderCopy(renderer, crosshairTexture, NULL, &mousePosRect);

	SDL_RenderPresent(renderer);
}

void Game::Shutdown() {
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(crosshairTexture);
	SDL_DestroyTexture(targetTexture);
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(timerTexture);
	SDL_DestroyTexture(scoreTierTexture);
	SDL_DestroyTexture(speedTierTexture);
	SDL_DestroyTexture(startButtonTexture);
	SDL_DestroyTexture(quitButtonTexture);
	SDL_DestroyTexture(backToMenuButtonTexture);

	Mix_FreeChunk(clickChunk);
	Mix_FreeChunk(shootChunk);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* Game::LoadTexture(string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		cout << "Failed to load surface: " << IMG_GetError() << endl;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (newTexture == NULL) {
		cout << "Failed to convert to texture: " << SDL_GetError() << endl;
	}

	SDL_FreeSurface(tempSurface);

	return newTexture;
}

SDL_Texture* Game::ConvertTextToTexture(string text, SDL_Rect* targetRect, bool center) {
	SDL_Surface* tempSurface = TTF_RenderText_Solid(textFont, text.c_str(), textColor);
	if (tempSurface == NULL) {
		cout << "Failed to load surface: " << TTF_GetError() << endl;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (newTexture == NULL) {
		cout << "Failed to convert to texture: " << SDL_GetError() << endl;
	}

	targetRect->w = tempSurface->w;
	targetRect->h = tempSurface->h;

	if (center) {
		targetRect->x = SCREEN_WIDTH / 2 - targetRect->w / 2;
	}

	SDL_FreeSurface(tempSurface);

	return newTexture;
}