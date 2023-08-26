#include "terrain/Structures.h"

House::House(vec2d pos) { this->pos = pos; }

void House::enter(std::shared_ptr<StaticBody> body)
{
	if (std::find(bodies_inside.begin(), bodies_inside.end(), body) == bodies_inside.end())
        bodies_inside.push_back(body);
}

void House::exit(std::shared_ptr<StaticBody> body)
{
	auto new_end = std::remove_if(bodies_inside.begin(), bodies_inside.end(),
	    [body](const std::shared_ptr<StaticBody>& element) {
	        return element == body;
	    });
    bodies_inside.erase(new_end, bodies_inside.end());
}

void House::draw_inside(SDL_Renderer* renderer, vec2d origin)
{
	float ScreenCenterX = 600;
	float ScreenCenterY = 350;
	
	for (auto entity : bodies_inside) {
		// if (std::dynamic_pointer_cast<Person>(entity)) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    		SDL_RenderFillCircle(renderer, ScreenCenterX+entity->pos.x-origin.x, ScreenCenterY+entity->pos.y-origin.y, 10);
    	// }
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillCircle(renderer, ScreenCenterX+exit_door.x-origin.x, ScreenCenterY+exit_door.y-origin.y, 10);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillCircle(renderer, ScreenCenterX, ScreenCenterY, 10);
}
