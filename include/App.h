#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "World.h"
#include "visuals/Animals.h"
#include "visuals/Chunks.h"
#include "visuals/Plants.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

class App
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    World world;
    AnimalVisual av;
    WaterVisual wv;
    TreeVisual tv;

    SDL_Event event;

    unsigned int fps = 60;

    void update();
    void draw();
public:
    App();
    void run();
};
