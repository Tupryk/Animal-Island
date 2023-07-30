#include <vector>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

class App
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    void update();
    void draw();
public:
    App();
    void run();
};
