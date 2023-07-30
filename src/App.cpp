#include "App.h"


App::App()
{
    
}

void App::update()
{

}

void App::draw()
{
    
}

void App::run()
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    SDL_Event event;

    while (1)
    {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		SDL_RenderClear(renderer);

		update();
		draw();

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
