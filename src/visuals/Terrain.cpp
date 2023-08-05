#include "visuals/Terrain.h"

void displayTerrain(SDL_Renderer* renderer, Terrain* terrain)
{
	float width = 420/terrain->dimensions;
	float height = 420/terrain->dimensions;

	for (int i = 0; i < terrain->dimensions; i++) {
	for (int j = 0; j < terrain->dimensions; j++)
	{
		SDL_Rect rect;
	    rect.x = i*width;
	    rect.y = j*height;
	    rect.w = width;
	    rect.h = height;

		if (terrain->chunks[i][j].type == ChunkTypes::SEA)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

		else if (terrain->chunks[i][j].type == ChunkTypes::GRASS
			|| terrain->chunks[i][j].type == ChunkTypes::VALLEY)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		else if (terrain->chunks[i][j].type == ChunkTypes::SAND)
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

		else SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);

		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (auto tree : terrain->chunks[i][j].trees)
			SDL_RenderFillCircle(renderer, rect.x+(tree.pos.x*width/terrain->chunks[i][j].size), rect.y+(tree.pos.y*height/terrain->chunks[i][j].size), 1);
	}}
}
