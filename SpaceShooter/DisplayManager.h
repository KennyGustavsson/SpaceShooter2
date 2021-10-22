#pragma once
#include "Mesh.h"

class DisplayManager 
{
public:
	SDL_Renderer* Renderer;
	SDL_Window* Window;
	int WindowWidth;
	int WindowHeight;

	/// <summary>
	/// Creates a window and renderer
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	DisplayManager(int width, int height)
	{
		int flags = 0;

		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING == -1))
		{
			std::cout << SDL_GetError() << std::endl;
		}

		// Create Window
		Window = SDL_CreateWindow("Asteroid Belt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (Window == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
		}

		// Create Renderer
		Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
		if (Renderer == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
		}

		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

		WindowWidth = width;
		WindowHeight = height;
	}

	~DisplayManager() 
	{
		SDL_DestroyRenderer(Renderer);
		SDL_DestroyWindow(Window);
		SDL_Quit();
	}

	/// <summary>
	/// Clears the renderer and sets the rendering color to default.
	/// </summary>
	void ClearRenderer() 
	{
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);
	}

	/// <summary>
	/// Present the frame to the window.
	/// </summary>
	void PresentRenderer() 
	{
		SDL_RenderPresent(Renderer);
	}
};