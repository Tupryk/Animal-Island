#include "visuals/Terrain.h"

void displayTerrain(SDL_Renderer* renderer, Terrain* terrain)
{
	float width = 600/terrain->dimensions;
	float heigth = 400/terrain->dimensions;

	for (int i = 0; i < terrain->dimensions; i++) {
	for (int j = 0; j < terrain->dimensions; j++)
	{
		SDL_Rect rect;
	    rect.x = i*width;
	    rect.y = j*heigth;
	    rect.w = width;
	    rect.h = heigth;

		if (terrain->chunks[i][j].type == ChunkTypes::SEA)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

		else if (terrain->chunks[i][j].type == ChunkTypes::LAND)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		else SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);

		SDL_RenderFillRect(renderer, &rect);
	}}
}
