#pragma once

#include <SDL2/SDL.h>

struct vec2d
{
	float x, y;

	vec2d(float x=0, float y=0) : x(x), y(y) {}

	vec2d operator+(const vec2d& other) const {
        return vec2d(this->x + other.x, this->y + other.y);
    }

    vec2d operator-(const vec2d& other) const {
        return vec2d(this->x - other.x, this->y - other.y);
    }

   	vec2d operator*(const float& scaler) const {
        return vec2d(this->x * scaler, this->y * scaler);
    }

    vec2d operator/(const float& scaler) const {
        return vec2d(this->x / scaler, this->y / scaler);
    }

    float get_length() {
    	return sqrt(x*x + y*y);
    }
};

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
