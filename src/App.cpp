#include "App.h"

App::App() : world(), av() { srand( time( NULL ) ); }

void App::update()
{
	//world.update();
	av.update();
}

void App::draw()
{
 	//world.draw(renderer);
 	av.draw(renderer);
}

void App::run()
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	while (true)
	{
	 	if (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() > 1000/fps)
	 	{
	 		begin = std::chrono::steady_clock::now();

			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					break;
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_k)
	                   world.display_stats = true;
	              	else if (event.key.keysym.sym == SDLK_q)
	              		world.display_stats = false;
				}
			}

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
