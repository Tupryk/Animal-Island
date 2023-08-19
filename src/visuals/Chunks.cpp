#include "visuals/Chunks.h"

WaterVisual::WaterVisual() : size(100, 100), pos(0, 0)
{
	sep_x = size.x / points_dim;
	sep_y = size.y / points_dim;
	for (int i = 0; i < points_dim; i++)
		for (int j = 0; j < points_dim; j++) {
			grid[i][j] = rand()%100;
			reverse[i][j] = rand()%2 == 0;
		}
}

void WaterVisual::setPos(vec2d pos) { this->pos = pos; }

void WaterVisual::update()
{
	for (int i = 0; i < points_dim; i++)
		for (int j = 0; j < points_dim; j++) {
			if (reverse[i][j]) grid[i][j] -= speed;
			else grid[i][j] += speed;
			if (grid[i][j] >= 100) reverse[i][j] = true;
			if (grid[i][j] <= 0) reverse[i][j] = false;
		}
}

void WaterVisual::draw(SDL_Renderer* renderer)
{
	float ScreenCenterX = 600;
	float ScreenCenterY = 350;

	for (int i = 0; i < points_dim-1; i++)
		for (int j = 0; j < points_dim-1; j++)
		{
			Sint16 x0 = i*sep_x+pos.x;
			Sint16 x1 = (i+1)*sep_x+pos.x;
			Sint16 y0 = j*sep_y+pos.y;
			Sint16 y1 = (j+1)*sep_y+pos.y;
			Sint16 vertsx[3] = {x0, x1, x1};
			Sint16 vertsy[3] = {y0, y0, y1};

			float bx = -sep_x;
			float by = 0;
			float bz = grid[i][j]-grid[i+1][j];

			float ax = 0;
			float ay = sep_y;
			float az = grid[i+1][j+1]-grid[i+1][j];

            float normx = ay*bz-az*by;
            float normy = az*bx-ax*bz;
            float normz = ax*by-ay*bx;
            float len_norm = sqrt(normx*normx+normy*normy+normz*normz);
            normx /= len_norm;
            normy /= len_norm;
            normz /= len_norm;
            float angle = (normz/sqrt(normx*normx+normy*normy+normz*normz)+1)/2;

            for (int k = 0; k < 3; k++) {
				float vz = getZfromY(vertsy[k], ScreenCenterY*2);
				vertsx[k] *= vz;
				vertsy[k] *= vz;
				vertsx[k] += ScreenCenterX;
				vertsy[k] += ScreenCenterY;
            }

			filledPolygonRGBA(renderer, vertsx, vertsy, 3, 100*angle, 100*angle, 255*angle, 255);

			vertsx[0] = x0;
			vertsx[1] = x0;
			vertsx[2] = x1;

			vertsy[0] = y0;
			vertsy[1] = y1;
			vertsy[2] = y1;

			ax = 0;
			ay = -sep_y;
			az = grid[i][j]-grid[i][j+1];

			bx = sep_x;
			by = 0;
			bz = grid[i+1][j+1]-grid[i][j+1];

			normx = ay*bz-az*by;
            normy = az*bx-ax*bz;
            normz = ax*by-ay*bx;
            len_norm = sqrt(normx*normx+normy*normy+normz*normz);
            normx /= len_norm;
            normy /= len_norm;
            normz /= len_norm;
            angle = (normz/sqrt(normx*normx+normy*normy+normz*normz)+1)/2;

            for (int k = 0; k < 3; k++) {
				float vz = getZfromY(vertsy[k], ScreenCenterY*2);
				vertsx[k] *= vz;
				vertsy[k] *= vz;
				vertsx[k] += ScreenCenterX;
				vertsy[k] += ScreenCenterY;
            }

			filledPolygonRGBA(renderer, vertsx, vertsy, 3, 100*angle, 100*angle, 255*angle, 255);
		}
}
