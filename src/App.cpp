#include "App.h"

App::App() : world(), av(), wv(), tv() { srand( time( NULL ) ); }

void App::update()
{
	world.update();
	//av.update();
	//wv.update(vec2d(-100, -100));
	//tv.update();
	//tv.setPos(vec2d(WINDOW_WIDTH*.5, WINDOW_HEIGHT*.8));
	//tv.setScale(20);
}

void App::draw()
{
 	world.draw(renderer);
 	//av.draw(renderer);
 	//wv.draw(renderer);
 	//tv.draw(renderer);
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
	               	else if (event.key.keysym.sym == SDLK_m)
	              		world.big_map = true;
	              	else if (event.key.keysym.sym == SDLK_q) {
	              		world.display_stats = false;
	              		world.big_map = false;
	              	}
	              	else {
	              		if (event.key.keysym.sym != SDLK_d && event.key.keysym.sym != SDLK_a)
	              			world.player.acc.x = 0;
	              		else {
	              			if (event.key.keysym.sym == SDLK_a)
		              			world.player.acc.x = -world.player.speed;
		              		if (event.key.keysym.sym == SDLK_d)
		              			world.player.acc.x = world.player.speed;
	              		}
	              		if (event.key.keysym.sym != SDLK_w && event.key.keysym.sym != SDLK_s)
	              			world.player.acc.y = 0;
	              		else {
	              			if (event.key.keysym.sym == SDLK_w)
		              			world.player.acc.y = -world.player.speed;
		              		if (event.key.keysym.sym == SDLK_s)
		              			world.player.acc.y = world.player.speed;
	              		}
	              	}
				} else {
					world.player.acc.x = 0;
					world.player.acc.y = 0;
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
