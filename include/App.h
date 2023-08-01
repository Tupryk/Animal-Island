#pragma once

#include <SDL2/SDL.h>

#include "visuals/Animals.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

class App
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    AnimalVisual animvis;

    unsigned int fps = 60;

    void update();
    void draw();
public:
    App();
    void run();
};
