#include "World.h"

World::World()
{
	srand( time( NULL ) );

	float radious = dimensions*.25;
	float center_point_xy = dimensions*.5;

	int island_seed = rand()%100000000;

	float valley_radious = dimensions*.1;
	float valley_center_x = center_point_xy+(dimensions*.05-rand()%static_cast<int>(dimensions*.1));
	float valley_center_y = center_point_xy+(dimensions*.05-rand()%static_cast<int>(dimensions*.1));

	int valley_seed = rand()%100000000;

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		vec2d dist_vec(i-center_point_xy, j-center_point_xy);
		float perlin_index = 10*dist_vec.get_degree()/(360);
		float rad = radious+perlinNoise1D(perlin_index, island_seed)*radious/16;

		if (dist_vec.get_length() <= rad)
		{
			dist_vec.x = i-valley_center_x;
			dist_vec.y = j-valley_center_y;
			perlin_index = 10*dist_vec.get_degree()/(360);
			rad = valley_radious+perlinNoise1D(perlin_index, valley_seed)*radious/16;

			if (dist_vec.get_length() <= rad)
				chunks[i][j] = Chunk(ChunkTypes::VALLEY);
			else
				chunks[i][j] = Chunk(ChunkTypes::GRASS);
		}
		else if (dist_vec.get_length() <= rad*1.2)
			chunks[i][j] = Chunk(ChunkTypes::SAND);
		else
			chunks[i][j] = Chunk(ChunkTypes::SEA);
		}}

	// Setup chunk neighbors
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		if (i != 0 && j != 0) chunks[i][j].neighbors[0] = &chunks[i-1][j-1];
		else chunks[i][j].neighbors[0] = NULL;

		if (j != 0) chunks[i][j].neighbors[1] = &chunks[i][j-1];
		else chunks[i][j].neighbors[1] = NULL;

		if (i != dimensions-1 && j != 0) chunks[i][j].neighbors[2] = &chunks[i+1][j-1];
		else chunks[i][j].neighbors[2] = NULL;

		if (i != 0) chunks[i][j].neighbors[3] = &chunks[i-1][j];
		else chunks[i][j].neighbors[3] = NULL;

		if (i != dimensions-1) chunks[i][j].neighbors[4] = &chunks[i+1][j];
		else chunks[i][j].neighbors[4] = NULL;

		if (i != 0 && j != dimensions-1) chunks[i][j].neighbors[5] = &chunks[i-1][j+1];
		else chunks[i][j].neighbors[5] = NULL;

		if (j != dimensions-1) chunks[i][j].neighbors[6] = &chunks[i][j+1];
		else chunks[i][j].neighbors[6] = NULL;

		if (i != dimensions-1 && j != dimensions-1) chunks[i][j].neighbors[7] = &chunks[i+1][j+1];
		else chunks[i][j].neighbors[7] = NULL;
	}}

	// Generate cats
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		if (chunks[i][j].type == ChunkTypes::VALLEY ||
			chunks[i][j].type == ChunkTypes::GRASS) {
			unsigned int num = rand()%100;

			if (num == 0) {
				float pos_x = rand()%chunk_size+(i*chunk_size);
				float pos_y = rand()%chunk_size+(j*chunk_size);

				cats.push_back(Cat({pos_x, pos_y}));
			}}}}

	// Generate squirrels
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		if (chunks[i][j].type == ChunkTypes::VALLEY ||
			chunks[i][j].type == ChunkTypes::GRASS) {
			unsigned int num = rand()%30;

			if (num == 0) {
				float pos_x = rand()%chunk_size+(i*chunk_size);
				float pos_y = rand()%chunk_size+(j*chunk_size);

				squirrels.push_back(Squirrel({pos_x, pos_y}));
				chunks[i][j].animals.push_back(&squirrels[squirrels.size()-1]);
			}}}}
	/*
	// Generate sharks
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		if (chunks[i][j].type == ChunkTypes::SEA) {
			unsigned int num = rand()%30;

			if (num == 0) {
				float pos_x = rand()%chunk_size+(i*chunk_size);
				float pos_y = rand()%chunk_size+(j*chunk_size);

				sharks.push_back(Shark({pos_x, pos_y}));
			}}}}
	*/
}

void World::update()
{
	for (int i = 0; i < dimensions; i++)
		for (int j = 0; j < dimensions; j++)
			chunks[i][j].update();

	std::vector<Squirrel*> pointerSquirrels;

    for (Squirrel& s : squirrels) pointerSquirrels.push_back(&s);

	for (int i = 0; i < cats.size(); i++) {
		unsigned int cx = cats[i].pos.x / static_cast<float>(chunk_size);
		unsigned int cy = cats[i].pos.y / static_cast<float>(chunk_size);

		AnimalState status = cats[i].update(chunks[cx][cy].neighbors, pointerSquirrels);
		if (status == AnimalState::DEAD) cats.erase(cats.begin() + i);
	}

	for (int i = 0; i < squirrels.size(); i++) {
		unsigned int cx = squirrels[i].pos.x / static_cast<float>(chunk_size);
		unsigned int cy = squirrels[i].pos.y / static_cast<float>(chunk_size);
		AnimalState status = squirrels[i].update(chunks[cx][cy].neighbors, pointerSquirrels);
		if (status == AnimalState::DEAD) squirrels.erase(squirrels.begin() + i);
		else if (status == AnimalState::HAD_CHILD) squirrels.push_back(Squirrel(squirrels[i].pos));
	}

	for (int i = 0; i < sharks.size(); i++) {
		unsigned int cx = sharks[i].pos.x / static_cast<float>(chunk_size);
		unsigned int cy = sharks[i].pos.y / static_cast<float>(chunk_size);
		sharks[i].update(chunks[cx][cy].neighbors);
	}
	if (KEEP_STATS) update_stats();
}

void World::draw(SDL_Renderer* renderer)
{
    if (display_stats) {
    	render_stats(renderer);
    	return; }

	float width = 420/dimensions;
	float height = 420/dimensions;

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		SDL_Rect rect;
	    rect.x = i*width;
	    rect.y = j*height;
	    rect.w = width;
	    rect.h = height;

	    /*
	    if (chunks[i][j].animals.size() > 0)
	    	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		else*/ if (chunks[i][j].type == ChunkTypes::SEA)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

		else if (chunks[i][j].type == ChunkTypes::GRASS)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		else if (chunks[i][j].type == ChunkTypes::VALLEY)
			SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);

		else if (chunks[i][j].type == ChunkTypes::SAND)
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

		else SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);

		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
		for (auto tree : chunks[i][j].trees)
			SDL_RenderFillCircle(renderer, rect.x+(tree.pos.x*width/chunk_size), rect.y+(tree.pos.y*height/chunk_size), 1);
	}}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (auto cat : cats)
		SDL_RenderFillCircle(renderer, cat.pos.x * width / chunk_size, cat.pos.y * height / chunk_size, 1);

	SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
	for (auto squirrel : squirrels)
		SDL_RenderFillCircle(renderer, squirrel.pos.x * width / chunk_size, squirrel.pos.y * height / chunk_size, 1);

	SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);
	for (auto shark : sharks)
		SDL_RenderFillCircle(renderer, shark.pos.x * width / chunk_size, shark.pos.y * height / chunk_size, 1);
}

void World::update_stats() {
	squirrel_population.push_back(squirrels.size());
	cat_population.push_back(cats.size());
}

void World::render_stats(SDL_Renderer* renderer)
{
	if (squirrel_population.size() < 64 || squirrel_population.size() != cat_population.size()) return;

	auto max_pointer = std::max_element(squirrel_population.begin(), squirrel_population.end());
	unsigned int squirrel_max_y = *max_pointer;

	max_pointer = std::max_element(cat_population.begin(), cat_population.end());
	unsigned int cat_max_y = *max_pointer;

	float max_y;
	squirrel_max_y > cat_max_y ? max_y = max_y = squirrel_max_y : max_y = cat_max_y;

	float W = 600;
	float H = 400;
	float x_section_size = W/static_cast<float>(cat_population.size());

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 64; i < cat_population.size(); i+=cat_population.size()/64)
		SDL_RenderDrawLine(renderer, (i-64)*x_section_size, cat_population[i-64]*H/max_y, i*x_section_size, cat_population[i]*H/max_y);

	SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
	for (int i = 64; i < squirrel_population.size(); i+=squirrel_population.size()/64)
		SDL_RenderDrawLine(renderer, (i-64)*x_section_size, squirrel_population[i-64]*H/max_y, i*x_section_size, squirrel_population[i]*H/max_y);
}
