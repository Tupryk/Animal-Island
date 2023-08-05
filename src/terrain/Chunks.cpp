#include "terrain/Chunks.h"

Chunk::Chunk(ChunkTypes type) : type(type) {}

Terrain::Terrain() : chunks{}
{
	float radious = dimensions*.25;
	float center_point_xy = dimensions*.5;

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		vec2d dist_vec(i-center_point_xy, j-center_point_xy);
		float perlin_index = 10*dist_vec.get_degree()/(360);
		float rad = radious+perlinNoise1D(perlin_index)*radious/16;
		// float rad = radious+sin(M_PI*2*(dist_vec.get_degree())/180*10)*5;
		// float rad = radious;
		if (dist_vec.get_length() <= rad)
			chunks[i][j] = Chunk(ChunkTypes::LAND);
		else if (dist_vec.get_length() <= rad*1.2)
			chunks[i][j] = Chunk(ChunkTypes::SAND);
		else
			chunks[i][j] = Chunk(ChunkTypes::SEA);
		}}
}
