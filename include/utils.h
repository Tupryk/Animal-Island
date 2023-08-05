#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
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

    float get_degree() {
        double radians = std::atan2(y, x);
        double degrees = radians * (180.0 / M_PI);
        degrees = fmod(degrees, 360.0);
        if (degrees < 0) {
            degrees += 360.0;
        }
        return degrees;
    }
};

double lerp(double a, double b, double t);
double grad(int hash, double x);
double perlinNoise1D(double x, int seed);

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
