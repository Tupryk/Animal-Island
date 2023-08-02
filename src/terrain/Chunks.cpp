#include "terrain/Chunks.h"

Chunk::Chunk(ChunkTypes type) : type(type) {}

Terrain::Terrain() : chunks{}
{
	float radious = 24;
	float center_point_xy = dimensions*.5;

	for (int i = 0; i < dimensions; i++) {
	for (int j = 0; j < dimensions; j++)
	{
		vec2d dist_vec(i-center_point_xy, j-center_point_xy);
		if (dist_vec.get_length() <= radious)
			chunks[i][j] = Chunk(ChunkTypes::LAND);
		else
			chunks[i][j] = Chunk(ChunkTypes::SEA);
		}}
}
