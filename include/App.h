#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "World.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

class App
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    World world;

    unsigned int fps = 60;

    void update();
    void draw();
public:
    App();
    void run();
};
