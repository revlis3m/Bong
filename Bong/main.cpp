#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

int main(int argc, char* argv[])
{
	srand(time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init, SDL error : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error on init ttf, SDL error : %s", TTF_GetError());
		return EXIT_FAILURE;
	}

	if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) // création de la configuration de la carte son
	{
		SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
		SDL_Quit();
		return -1;
	}

	//Mix_Music* gameMusic = Mix_LoadMUS("Assets/main theme.mp3");
	//Mix_PlayMusic(gameMusic, -1);

	TTF_Font* gameFont = nullptr;
	gameFont = TTF_OpenFont("Assets/impact.ttf", 32);
	if (gameFont < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
	}

	SDL_Window* windowBong = nullptr;
	SDL_Renderer* renderBong = nullptr;
	windowBong = SDL_CreateWindow("Bong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (windowBong < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in creating window, SDL error : %s", SDL_GetError());
		return EXIT_FAILURE;
	}
	renderBong = SDL_CreateRenderer(windowBong, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderBong < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in creating render, SDL error : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Surface* backgroundSurface = IMG_Load("Assets/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderBong, backgroundSurface);

	SDL_Rect player1 = { 0,208,32,192 };

	SDL_Rect player2 = { 768,208,32,192 };

	SDL_Rect padle = { 368,268,32,32 };

	SDL_Rect arena = { 0,0,800,600 };

	SDL_Surface* playersurface = IMG_Load("Assets/bottle.png");
	if (playersurface < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in creating surface, SDL error : %s", IMG_GetError());
		return EXIT_FAILURE;
	}
	SDL_Texture* playerTexture1 = nullptr;
	SDL_Texture* playerTexture2 = nullptr;
	playerTexture1 = SDL_CreateTextureFromSurface(renderBong, playersurface);
	playerTexture2 = SDL_CreateTextureFromSurface(renderBong, playersurface);

	SDL_Surface* padleSurface = IMG_Load("Assets/circle.png");
	SDL_Texture* padleTexture = SDL_CreateTextureFromSurface(renderBong, padleSurface);

	SDL_Surface* icon = nullptr;
	icon = IMG_Load("Assets/icon.png");

	SDL_SetWindowIcon(windowBong, icon);

	string score1string = "0";
	string score2string = "0";

	SDL_Surface* gameScore1Surface = nullptr;
	SDL_Surface* gameScore2Surface = nullptr;
	SDL_Surface* gameScore_Surface = nullptr;

	SDL_Texture* gameScore1Texture = nullptr;
	SDL_Texture* gameScore2Texture = nullptr;
	SDL_Texture* gameScore_Texture = nullptr;

	SDL_Rect score1rect = { 350,0,32,32 };
	SDL_Rect score2rect = { 410,0,32,32 };
	SDL_Rect score_rect = { 380,0,32,32 };

	SDL_Point a1 = { 0,0 }, a2 = { 0,600 }, b1 = { 800,0 }, b2 = {800,600};


	SDL_SetRenderDrawBlendMode(renderBong, SDL_BLENDMODE_NONE);

	int score1 = 0;//Score player1;
	int score2 = 0;//Score player2;

	bool start = true;
	bool gamestart = true;
	bool mainmenu = false;

	SDL_Event event;
	bool running = SDL_TRUE;

	int mVx = rand() %4 +1, mVy = rand() %4 +1;
	const int Vx = mVx, Vy = mVy;
	while (running)
	{

		gameScore1Surface = TTF_RenderText_Blended(gameFont, score1string.c_str(), SDL_Color{ 255,255,255,255 });
		gameScore2Surface = TTF_RenderText_Blended(gameFont, score2string.c_str(), SDL_Color{ 255,255,255,255 });
		gameScore_Surface = TTF_RenderText_Blended(gameFont, " - ", SDL_Color{ 255,255,255,255 });

		gameScore1Texture = SDL_CreateTextureFromSurface(renderBong, gameScore1Surface);
		gameScore2Texture = SDL_CreateTextureFromSurface(renderBong, gameScore2Surface);
		gameScore_Texture = SDL_CreateTextureFromSurface(renderBong, gameScore_Surface);

		//Velocity of padle
		padle.x += mVx;
		padle.y += mVy;

		if (padle.x >= 799)
		{
			score1++;
			score1string = to_string(score1);
			padle = { 368,268,32,32 };
			Mix_Music* Victory = Mix_LoadMUS("Assets/game over.mp3");
			Mix_PlayMusic(Victory, 0);
		}

		if (padle.x <= 1)
		{
			score2++;
			score2string = to_string(score2);
			padle = { 368,268,32,32 };
			Mix_Music* Victory = Mix_LoadMUS("Assets/game over.mp3");
			Mix_PlayMusic(Victory, 0);
		}

		if (padle.y <= 0)
		{
			if (mVy < 10 || mVy > -10)
			{
				mVy = -mVy * 1.25;
			}
			Mix_Music* impactSound = Mix_LoadMUS("Assets/Hit_Hurt.wav");
			Mix_PlayMusic(impactSound, 0);
		}

		if (padle.y >= 568)
		{
			if (mVy < 10 || mVy > -10)
			{
				mVy = -mVy * 1.25;
			}
			Mix_Music* impactSound = Mix_LoadMUS("Assets/Hit_Hurt.wav");
			Mix_PlayMusic(impactSound, 0);
		}

		if (SDL_HasIntersection(&padle, &player1))
		{
			if (mVx < 10 || mVx > -10)
			{
				mVx = -mVx * 1.25;
			}
			Mix_Music* impactSound = Mix_LoadMUS("Assets/Hit_Hurt.wav");
			Mix_PlayMusic(impactSound, 0);
		}

		if (SDL_HasIntersection(&padle, &player2))
		{
			if (mVx < 10 || mVx > -10)
			{
				mVx = -mVx * 1.25;
			}
			Mix_Music* impactSound = Mix_LoadMUS("Assets/Hit_Hurt.wav");
			Mix_PlayMusic(impactSound, 0);
		}

		Mix_Music* gameMusic = Mix_LoadMUS("Assets/main theme.mp3");

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					running = SDL_FALSE;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					if (player1.y >= 0)
					{
						player1.y -= 4;
					}
					break;
				case SDLK_s:
					if (player1.y <= 408)
					{
						player1.y += 4;
					}
					break;
				case SDLK_UP:
					if (player2.y >= 0)
					{
						player2.y -= 4;
					}
					break;
				case SDLK_DOWN:
					if (player2.y <= 408)
					{
						player2.y += 4;
					}
					break;
				case SDLK_SPACE:
					start = true;
					break;
				case SDLK_r:
				{
					score1 = 0;
					score2 = 0;
				}
				break;
				case SDLK_ESCAPE:
				{
					start = false;
					break;
				}
				}
			}
		}

		SDL_SetRenderDrawColor(renderBong, 13, 198, 198, 255);
		SDL_RenderClear(renderBong);

		//SDL_RenderCopy(renderBong, backgroundTexture, NULL, NULL);
		SDL_RenderCopy(renderBong, NULL, NULL, &arena);
		SDL_RenderCopy(renderBong, playerTexture1, NULL, &player1);
		SDL_RenderCopy(renderBong, playerTexture2, NULL, &player2);
		SDL_RenderCopy(renderBong, padleTexture, NULL, &padle);
		SDL_RenderCopy(renderBong, gameScore1Texture, NULL, &score1rect);
		SDL_RenderCopy(renderBong, gameScore2Texture, NULL, &score2rect);
		SDL_RenderCopy(renderBong, gameScore_Texture, NULL, &score_rect);

		SDL_RenderPresent(renderBong);

		if (!start)
		{
			Mix_PlayMusic(gameMusic, -1);
		}

		//Pausing the game
		while (start == false)
		{
			mVx = 0;
			mVy = 0;

			if (SDL_PollEvent(&event))
			{
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					running = SDL_FALSE;
					start = true;
				}
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					start = true;
					mVx = Vx;
					mVy = Vy;
					Mix_PauseMusic();
				}
			}
			SDL_Rect pauseSign = { 340,200,100,56 };
			SDL_Surface* pauseSignSurface = TTF_RenderText_Blended(gameFont, "PAUSE", SDL_Color{ 255,255,255,255 });
			SDL_Texture* pauseSignTexture = SDL_CreateTextureFromSurface(renderBong, pauseSignSurface);

			SDL_RenderCopy(renderBong, pauseSignTexture, NULL, &pauseSign);
			SDL_RenderPresent(renderBong);

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				SDL_FreeSurface(pauseSignSurface);
				SDL_DestroyTexture(pauseSignTexture);
			}
		}

	}
	SDL_FreeSurface(icon);
	SDL_FreeSurface(padleSurface);
	SDL_FreeSurface(playersurface);
	SDL_FreeSurface(gameScore1Surface);
	SDL_FreeSurface(gameScore2Surface);
	SDL_FreeSurface(gameScore_Surface);
	SDL_DestroyTexture(playerTexture1);
	SDL_DestroyTexture(padleTexture);
	SDL_DestroyTexture(playerTexture2);
	SDL_DestroyTexture(gameScore1Texture);
	SDL_DestroyTexture(gameScore2Texture);
	SDL_DestroyTexture(gameScore_Texture);
	SDL_DestroyRenderer(renderBong);
	SDL_DestroyWindow(windowBong);
	TTF_CloseFont(gameFont);
	TTF_Quit();
	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}