#include "App.h"

App::App() : animvis()
{
    
}

void App::update()
{
	animvis.update();
}

void App::draw()
{
 	animvis.draw(renderer);
}

void App::run()
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

	SDL_Event event;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	 while (true)
	 {
	 	if (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() > 1000/fps)
	 	{
	 		begin = std::chrono::steady_clock::now();

			if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
				break;
			SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
			SDL_RenderClear(renderer);

			update();
			draw();

			SDL_RenderPresent(renderer);

			end = std::chrono::steady_clock::now();
		}
		else end = std::chrono::steady_clock::now();
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
