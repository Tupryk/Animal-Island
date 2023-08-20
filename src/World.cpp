#include "World.h"

World::World() : player(vec2d(2000, 3232))
{
	srand( time( NULL ) );

	float radious = dimensions*.25;
	float center_point_xy = dimensions*.5;

	int island_seed = rand();

	float valley_radious = dimensions*.1;
	float valley_center_x = center_point_xy+(dimensions*.05-rand()%static_cast<int>(dimensions*.1));
	float valley_center_y = center_point_xy+(dimensions*.05-rand()%static_cast<int>(dimensions*.1));

	int valley_seed = rand();

	// Generate Chunks
	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		vec2d dist_vec(i-center_point_xy, j-center_point_xy);
		float perlin_index = 10*dist_vec.get_degree()/(360);
		float rad = radious+perlinNoise1D(perlin_index, island_seed)*radious/16;

		vec2d coor(i*chunk_size, j*chunk_size);

		if (dist_vec.get_length() <= rad)
		{
			dist_vec.x = i-valley_center_x;
			dist_vec.y = j-valley_center_y;
			perlin_index = 10*dist_vec.get_degree()/(360);
			rad = valley_radious+perlinNoise1D(perlin_index, valley_seed)*radious/16;

			if (dist_vec.get_length() <= rad)
				chunks[i][j] = Chunk(ChunkTypes::VALLEY, coor);
			else
				chunks[i][j] = Chunk(ChunkTypes::GRASS, coor);
		}
		else if (dist_vec.get_length() <= rad*1.2)
			chunks[i][j] = Chunk(ChunkTypes::SAND, coor);
		else
			chunks[i][j] = Chunk(ChunkTypes::SEA, coor);
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

		if (i != dimensions-1) chunks[i][j].neighbors[5] = &chunks[i+1][j];
		else chunks[i][j].neighbors[5] = NULL;

		if (i != 0 && j != dimensions-1) chunks[i][j].neighbors[6] = &chunks[i-1][j+1];
		else chunks[i][j].neighbors[6] = NULL;

		if (j != dimensions-1) chunks[i][j].neighbors[7] = &chunks[i][j+1];
		else chunks[i][j].neighbors[7] = NULL;

		if (i != dimensions-1 && j != dimensions-1) chunks[i][j].neighbors[8] = &chunks[i+1][j+1];
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
    		} else if (num < 7) {
    			std::shared_ptr<Squirrel> squirrel = std::make_shared<Squirrel>(vec2d(pos_x, pos_y));
    			animals.push_back(squirrel);
    		}}}}
}

std::shared_ptr<Animal> World::createAnimalCopy(const std::shared_ptr<Animal>& animal) {
    if (std::dynamic_pointer_cast<Cat>(animal)) {
        return std::make_shared<Cat>(*std::dynamic_pointer_cast<Cat>(animal));
    } else if (std::dynamic_pointer_cast<Squirrel>(animal)) {
        return std::make_shared<Squirrel>(*std::dynamic_pointer_cast<Squirrel>(animal));
    } else {
        return std::make_shared<Animal>(*animal);
    }
}

AnimalState World::update_animal(const std::shared_ptr<Animal>& animal)
{
    unsigned int cx = animal->pos.x / static_cast<float>(chunk_size);
	unsigned int cy = animal->pos.y / static_cast<float>(chunk_size);

	std::vector<std::shared_ptr<Animal>> animals_viewed;
	std::vector<std::shared_ptr<Tree>> plants_viewed;
	std::vector<Chunk*> chunks_viewed = get_chunks_viewed(animal->fov, animal->see_distance, animal->pos, animal->acc);
	for (auto chunk : chunks_viewed) {
	    animals_viewed.insert(animals_viewed.end(), chunk->animals.begin(), chunk->animals.end());
	    plants_viewed.insert(plants_viewed.end(), chunk->trees.begin(), chunk->trees.end());
	}

	AnimalState status = animal->update(chunks[cx][cy].neighbors, animals_viewed, plants_viewed, brightness);

	// Temporary code to avoid animals from going to the sea
    unsigned int ncx = animal->pos.x / static_cast<float>(chunk_size);
	unsigned int ncy = animal->pos.y / static_cast<float>(chunk_size);
    if (chunks[ncx][ncy].type == ChunkTypes::SEA) animal->pos = vec2d(cx*chunk_size, cy*chunk_size);

    return status;
}

void World::update()
{
	// Update Day Year Cycle
	update_time();

	// Update Player
	player.update_pos();

	// Update Animals
	std::vector<std::shared_ptr<Animal>> updated_animals;
	std::vector<std::thread> threads;
	for (int i = 0; i < animals.size(); i++) {
	    threads.emplace_back([this, &updated_animals, i]()
	    {
	    	std::shared_ptr<Animal> anim_copy = createAnimalCopy(animals[i]);
            AnimalState status = update_animal(anim_copy);
            if (status != AnimalState::DEAD) {
                std::lock_guard<std::mutex> lock(updated_animals_mutex);
                updated_animals.push_back(anim_copy);
                if (status == AnimalState::HAD_CHILD)
                	updated_animals.push_back(anim_copy->build_child());
            }
        });
	}

	for (std::thread& thread : threads)
	    thread.join();

	animals.clear();
	for (const auto& updated_animal : updated_animals)
	    animals.push_back(updated_animal);
    
    // Update Chunks
    for (int i = 0; i < dimensions; i++)
		for (int j = 0; j < dimensions; j++)
			chunks[i][j].animals.clear();

	 for (auto animal_ptr : animals) {
        unsigned int cx = animal_ptr->pos.x / static_cast<float>(chunk_size);
		unsigned int cy = animal_ptr->pos.y / static_cast<float>(chunk_size);
		chunks[cx][cy].animals.push_back(animal_ptr);
	}

	// Store World data
	#if KEEP_STATS
		update_stats();
	#endif
}

void World::update_time()
{
	day_cycle++;
	if (day_cycle >= day_duration) {
		day_cycle = 0;
		year_cycle++;
		if (year_cycle >= year_duration) year_cycle = 0;
	}
	sun_angle = sin(day_cycle*M_PI*2/day_duration);
	brightness = sun_angle;
	if (sun_angle < .2) brightness = .2;
}

void World::draw(SDL_Renderer* renderer)
{
    if (display_stats) {
    	render_stats(renderer);
    	return; }
    draw_world(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*(1-brightness)); // Dark color with transparency
    SDL_RenderFillRect(renderer, nullptr);

    draw_mini_map(renderer);
}

void World::draw_world(SDL_Renderer* renderer)
{
	float ScreenCenterX = 600;
	float ScreenCenterY = 350;

	int render_width = 15; // in chunks (Should not be divisible by 2)
	int render_height = 9; // in chunks (Should not be divisible by 2)
	int render_offset_x = 0;
	int render_offset_y = -1;

	unsigned int pcx = player.pos.x / static_cast<float>(chunk_size);
	unsigned int pcy = player.pos.y / static_cast<float>(chunk_size);

	// Floor
	water_visual.update();
	for (int j = -.5*(render_height-1)+render_offset_y; j <= render_height*.5+render_offset_y; j++) {
		float gradienty = (.5*(render_height-1) + j)/render_height*.2+.8;
		for (int i = -.5*(render_width-1)+render_offset_x; i <= render_width*.5+render_offset_x; i++)
		{
			float gradientx = (.2-(abs(i/(render_width*.5))*.2)+.8);
			// float brightness = sun_angle;
			// if (sun_angle < .2) brightness = .2;
			float gradient = gradientx*gradienty; //*brightness;

			float ox = chunks[pcx+i][pcy+j].coor.x-player.pos.x;
			float oy = chunks[pcx+i][pcy+j].coor.y-player.pos.y;
			float z = getZfromY(oy, ScreenCenterY*2);

			float x = ox;
			float y = oy;

			vec2d p0(ScreenCenterX+x*z, ScreenCenterY+y*z);
			vec2d p1(ScreenCenterX+(x+chunk_size)*z, ScreenCenterY+y*z);

			y += chunk_size;
			z = getZfromY(y, ScreenCenterY*2);
			vec2d p2(ScreenCenterX+(x+chunk_size)*z, ScreenCenterY+y*z);
			vec2d p3(ScreenCenterX+x*z, ScreenCenterY+y*z);

			Sint16 vertsx[4] = {
				static_cast<Sint16>(p0.x),
				static_cast<Sint16>(p1.x),
				static_cast<Sint16>(p2.x),
				static_cast<Sint16>(p3.x)};
		    Sint16 vertsy[4] = {
		    	static_cast<Sint16>(p0.y),
		    	static_cast<Sint16>(p1.y),
		    	static_cast<Sint16>(p2.y),
		    	static_cast<Sint16>(p3.y)};

		    if (chunks[pcx+i][pcy+j].type == ChunkTypes::SEA) {
		    	filledPolygonRGBA(renderer, vertsx, vertsy, 4, 0, 0, 255, 255);
		    	water_visual.setPos(vec2d(ox, oy));
				water_visual.draw(renderer);
		    }

			else if (chunks[pcx+i][pcy+j].type == ChunkTypes::GRASS)
				filledPolygonRGBA(renderer, vertsx, vertsy, 4, 100*gradient, 200*gradient, 0, 255);

			else if (chunks[pcx+i][pcy+j].type == ChunkTypes::VALLEY)
				filledPolygonRGBA(renderer, vertsx, vertsy, 4, 50*gradient, 150*gradient, 0, 255);

			else if (chunks[pcx+i][pcy+j].type == ChunkTypes::SAND)
				filledPolygonRGBA(renderer, vertsx, vertsy, 4, 194*gradient, 178*gradient, 128*gradient, 255);

			else filledPolygonRGBA(renderer, vertsx, vertsy, 4, 255*gradient, 192*gradient, 203*gradient, 255);

			#if DEBUG
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawLine(renderer, p0.x, p0.y, p1.x, p1.y);
				SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
				SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
				SDL_RenderDrawLine(renderer, p3.x, p3.y, p0.x, p0.y);
			#endif
		}
		for (int i = -.5*(render_width-1)+render_offset_x; i <= render_width*.5+render_offset_x; i++)
		for (auto tree : chunks[pcx+i][pcy+j].trees)
		{
			float tx = tree->pos.x-player.pos.x;
			float ty = tree->pos.y-player.pos.y;
			float tz = getZfromY(ty, ScreenCenterY*2);

			float size = 50;
			vec2d start(tx-size*.5, ty);
			vec2d end(tx+size*.5, ty);
			float visual_size = ((start-end)*tz).get_length();

			float gradientx = (.2-(abs(i/(render_width*.5))*.2)+.8);
			float brightness = sun_angle;
			if (sun_angle < .2) brightness = .2;
			float gradient = gradientx*gradienty*brightness;

			tree->visual.setScale(visual_size);
			tree->visual.setPos(vec2d(ScreenCenterX+tx*tz, ScreenCenterY+ty*tz));
			tree->visual.draw(renderer, gradient);
		}
		if (j == 0) {
			// Player
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    		SDL_RenderFillCircle(renderer, ScreenCenterX, ScreenCenterY, 10);
		}
	}

	// Animals
	for (const auto& animal : animals)
    {
    	float x = animal->pos.x - player.pos.x;
        float y = animal->pos.y - player.pos.y;

        if (abs(x) < ScreenCenterX && abs(y) < ScreenCenterY*.5) {
        	float z = getZfromY(y, ScreenCenterY*2);

        	float size = 10;
			vec2d start(x-size*.5, y);
			vec2d end(x+size*.5, y);
			float visual_size = ((start-end)*z).get_length();

	        if (std::dynamic_pointer_cast<Cat>(animal))
	            SDL_SetRenderDrawColor(renderer, 200*(animal->age/animal->max_age), 200*(animal->age/animal->max_age), 200*(animal->age/animal->max_age), 255);
	        else if (std::shared_ptr<Squirrel> squirrel = std::dynamic_pointer_cast<Squirrel>(animal)) {
	        	if (squirrel->on_tree) y -= 50;
	        	SDL_SetRenderDrawColor(renderer, 255*(1-static_cast<float>(animal->age)/static_cast<float>(animal->max_age)), 128*(1-static_cast<float>(animal->age)/static_cast<float>(animal->max_age)), 0, 255);
	        }
	        SDL_RenderFillCircle(renderer,  ScreenCenterX+x*z, ScreenCenterY+y*z, visual_size*.5);
	    }
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

void World::draw_mini_map(SDL_Renderer* renderer)
{
	float width;
	float height;

	if (big_map) {
    	width = 700/dimensions;
		height = 700/dimensions;
    } else {
		width = 210/dimensions;
		height = 210/dimensions;
	}

	#if DEBUG
		std::shared_ptr<Animal> animal = animals.front();
		std::vector<Chunk*> viewed = get_chunks_viewed(animal->fov, animal->see_distance, animal->pos, animal->acc);
	#endif

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		SDL_Rect rect;
	    rect.x = i*width;
	    rect.y = j*height;
	    rect.w = width;
	    rect.h = height;

    	#if DEBUG
    		auto it = std::find(viewed.begin(), viewed.end(), &chunks[i][j]);
    		if (it != viewed.end())
    			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	    else
	    #endif
	    if (chunks[i][j].type == ChunkTypes::SEA)
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
			SDL_RenderFillCircle(renderer, tree->pos.x*width/chunk_size, tree->pos.y*height/chunk_size, 1);
	}}

	for (const auto& animal : animals)
    {
        if (std::dynamic_pointer_cast<Cat>(animal))
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else if (std::dynamic_pointer_cast<Squirrel>(animal))
        	SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);

        SDL_RenderFillCircle(renderer, animal->pos.x * width / chunk_size, animal->pos.y * height / chunk_size, 1);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillCircle(renderer, player.pos.x * width / chunk_size, player.pos.y * height / chunk_size, 10);
}

Chunk* World::pos2chunk(vec2d pos) {
	return &chunks
		[static_cast<int>(pos.x / static_cast<float>(chunk_size))]
		[static_cast<int>(pos.y / static_cast<float>(chunk_size))];
}

std::vector<Chunk*> World::get_chunks_viewed(float fov, float distance, vec2d pos, vec2d dir)
{
	std::vector<Chunk*> viewed;
	vec2d origin(pos.x / static_cast<float>(chunk_size), pos.y / static_cast<float>(chunk_size));

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

	// Needs to be optimized (Maybe push back trinagle pos and remove this).
	for (int i = 0; i < 9; i++) {
	    Chunk* neighbor = chunks[static_cast<int>(origin.x)][static_cast<int>(origin.y)].neighbors[i];
	    auto it = std::find(viewed.begin(), viewed.end(), neighbor);
	    if (neighbor != NULL && it != viewed.end())
	        viewed.push_back(neighbor);
	}

	return viewed;
}
