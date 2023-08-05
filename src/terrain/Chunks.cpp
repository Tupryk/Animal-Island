#include "terrain/Chunks.h"

Chunk::Chunk(ChunkTypes type) : type(type)
{
	if (type == ChunkTypes::VALLEY) {
		unsigned int trees_count = rand()%5;

		for (int i = 0; i < trees_count; i++)
		{
			float pos_x = rand()%size;
			float pos_y = rand()%size;

			trees.push_back(Tree({pos_x, pos_y}));
		}
	}
}

void Chunk::update()
{
	for (int i = 0; i < trees.size(); i++)
		trees[i].update();
}

Terrain::Terrain() : chunks{}
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
}
