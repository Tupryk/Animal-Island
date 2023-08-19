#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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

    vec2d norm() {
        float length = get_length();
        return vec2d(x/length, y/length);
    }

    vec2d rotate(float degrees) {
        float radians = degrees * M_PI / 180.0f;
        return vec2d(x * cos(radians) - y * sin(radians),
                     x * sin(radians) + y * cos(radians));
    }
};

double lerp(double a, double b, double t);
double grad(int hash, double x);
double perlinNoise1D(double x, int seed);
float getZfromY(float y, float WindowHeight, float scaler=5, float stretch=.1);

float crossProduct(vec2d v1, vec2d v2);

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

int SDL_RenderFillAlmond(SDL_Renderer* renderer, vec2d pos, vec2d origin0, vec2d origin1, float radius0, float radius1, int r=0, int g=0, int b=0);
int SDL_RenderFillMoon(SDL_Renderer* renderer, vec2d pos, vec2d origin0, vec2d origin1, float radius0, float radius1, int r=0, int g=0, int b=0);
