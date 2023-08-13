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

		chunks[i][j].neighbors[4] = &chunks[i][j];

		if (i != dimensions-1) chunks[i][j].neighbors[4] = &chunks[i+1][j];
		else chunks[i][j].neighbors[5] = NULL;

		if (i != 0 && j != dimensions-1) chunks[i][j].neighbors[5] = &chunks[i-1][j+1];
		else chunks[i][j].neighbors[6] = NULL;

		if (j != dimensions-1) chunks[i][j].neighbors[6] = &chunks[i][j+1];
		else chunks[i][j].neighbors[7] = NULL;

		if (i != dimensions-1 && j != dimensions-1) chunks[i][j].neighbors[7] = &chunks[i+1][j+1];
		else chunks[i][j].neighbors[8] = NULL;
	}}

	// Generate animals
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		if (chunks[i][j].type == ChunkTypes::VALLEY ||
			chunks[i][j].type == ChunkTypes::GRASS) {
			unsigned int num = rand()%100;

			float pos_x = rand()%chunk_size+(i*chunk_size);
			float pos_y = rand()%chunk_size+(j*chunk_size);

			if (num == 0) {
				std::shared_ptr<Cat> cat = std::make_shared<Cat>(vec2d(pos_x, pos_y));
    			animals.push_back(cat);
    		} else if (num < 4) {
    			std::shared_ptr<Squirrel> squirrel = std::make_shared<Squirrel>(vec2d(pos_x, pos_y));
    			animals.push_back(squirrel);
    		}}}}
}

void World::update()
{
	for (int i = 0; i < dimensions; i++)
		for (int j = 0; j < dimensions; j++)
			chunks[i][j].update();

	std::vector<Animal*> animals_s;
   	
    for (auto anim = animals.begin(); anim != animals.end();) {
        unsigned int cx = (*anim)->pos.x / static_cast<float>(chunk_size);
		unsigned int cy = (*anim)->pos.y / static_cast<float>(chunk_size);

		AnimalState status = (*anim)->update(chunks[cx][cy].neighbors, animals_s);

		if (status == AnimalState::DEAD) {
			anim = animals.erase(anim);
			continue;
        }
        else if (status == AnimalState::HAD_CHILD) {
        	if (std::dynamic_pointer_cast<Squirrel>(*anim)) {
            	std::shared_ptr<Squirrel> child = std::make_shared<Squirrel>((*anim)->pos);
    			animals.push_back(child);
        	}
        }
        ++anim;
    }

    for (int i = 0; i < dimensions; i++)
		for (int j = 0; j < dimensions; j++)
			chunks[i][j].animals.clear();

	 for (const auto& animal : animals) {
        unsigned int cx = animal->pos.x / static_cast<float>(chunk_size);
		unsigned int cy = animal->pos.y / static_cast<float>(chunk_size);
		chunks[cx][cy].animals.push_back(animal.get());
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

	Animal* animal = animals.front().get();
    std::vector<Chunk*> viewed = get_chunks_viewed(animal->fov, animal->see_distance, animal->pos, animal->look_dir);

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		SDL_Rect rect;
	    rect.x = i*width;
	    rect.y = j*height;
	    rect.w = width;
	    rect.h = height;

	    auto it = std::find(viewed.begin(), viewed.end(), &chunks[i][j]);
    	if (it != viewed.end())
	    	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	    else if (chunks[i][j].type == ChunkTypes::SEA)
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

	for (const auto& animal : animals)
    {
        if (std::dynamic_pointer_cast<Cat>(animal))
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else if (std::dynamic_pointer_cast<Squirrel>(animal))
        	SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);

        SDL_RenderFillCircle(renderer, animal->pos.x * width / chunk_size, animal->pos.y * height / chunk_size, 1);
    }
}

void World::update_stats() {
	int squirrelCount = std::count_if(animals.begin(), animals.end(),
        [](const std::shared_ptr<Animal>& animal) {
            return std::dynamic_pointer_cast<Squirrel>(animal) != nullptr;
        });
	squirrel_population.push_back(squirrelCount);
    int catCount = std::count_if(animals.begin(), animals.end(),
        [](const std::shared_ptr<Animal>& animal) {
            return std::dynamic_pointer_cast<Cat>(animal) != nullptr;
        });
    cat_population.push_back(catCount);
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

vec2d World::pos2chunk(vec2d pos) {
	return vec2d(pos.x / static_cast<float>(chunk_size), pos.y / static_cast<float>(chunk_size));
}

std::vector<Chunk*> World::get_chunks_viewed(float fov, float distance, vec2d pos, vec2d dir)
{
	std::vector<Chunk*> viewed;
	vec2d origin = pos2chunk(pos);

	vec2d vt1 = origin;
	vec2d vt2 = origin + (dir.norm().rotate(-fov*.5) * distance*2);
	vec2d vt3 = origin + (dir.norm().rotate(fov*.5) * distance*2);

	// Barycentric Algorithm
	int minX = vt1.x;
	if (vt2.x < minX) minX = vt2.x;
	if (vt3.x < minX) minX = vt3.x;

	int minY = vt1.y;
	if (vt2.y < minY) minY = vt2.y;
	if (vt3.y < minY) minY = vt3.y;

	int maxX = vt1.y;
	if (vt2.x > maxX) maxX = vt2.x;
	if (vt3.x > maxX) maxX = vt3.x;

	int maxY = vt1.y;
	if (vt2.y > maxY) maxY = vt2.y;
	if (vt3.y > maxY) maxY = vt3.y;

	vec2d vs1(vt2.x - vt1.x, vt2.y - vt1.y);
	vec2d vs2(vt3.x - vt1.x, vt3.y - vt1.y);

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++)
		{
			vec2d q(x - vt1.x, y - vt1.y);

			float s = (float)crossProduct(q, vs2) / crossProduct(vs1, vs2);
			float t = (float)crossProduct(vs1, q) / crossProduct(vs1, vs2);

			vec2d dist = origin - vec2d(x, y);

			if ((s >= 0) && (t >= 0) && (s + t <= 1) && dist.get_length() < distance)
				viewed.push_back(&chunks[x][y]);
	  	}
	}

	return viewed;
}
