#include "utils.h"

// Function to perform linear interpolation
double lerp(double a, double b, double t) {
    return (1.0 - t) * a + t * b;
}

// Function to calculate gradient at given index in permutation table
double grad(int hash, double x) {
    int h = hash & 15;
    double grad = 1.0 + (h & 7); // Gradient value in range [1, 8]
    if (h & 8) {
        grad = -grad; // Randomly invert the gradient
    }
    return (grad * x); // Multiply the gradient with x to get the gradient component along x-axis
}

// Function to calculate 1D Perlin noise
double perlinNoise1D(double x, int seed)
{
    std::vector<int> permutationTable(256);
    std::iota(permutationTable.begin(), permutationTable.end(), 0);
    auto rng = std::default_random_engine{static_cast<std::default_random_engine::result_type>(seed)};
    std::shuffle(std::begin(permutationTable), std::end(permutationTable), rng);

    // Determine grid cell coordinates
    int X = (int)std::floor(x) & 255;
    // Relative x position in the grid cell
    x -= std::floor(x);

    // Compute fade curves for x
    double fade = x * x * x * (x * (x * 6 - 15) + 10);

    // Get the gradient noise contributions from the surrounding grid cells
    int a = permutationTable[X];
    int b = permutationTable[X + 1];

    // Compute the dot product between the gradient and distance vectors
    double gradA = grad(a, x);
    double gradB = grad(b, x - 1.0);

    // Interpolate the gradients along the x-axis
    double noise = lerp(gradA, gradB, fade);

    return noise;
}

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillAlmond(SDL_Renderer* renderer, int x, int y, int y0, int y1, int radius0, int radius1)
{
    int status = 0;

    for (int yOffset = y0; yOffset <= y1; ++yOffset) {
        // Calculate the corresponding x radius for each y offset
        int xRadius = radius0 + (radius1 - radius0) * (yOffset - y0) / (y1 - y0);

        status += SDL_RenderDrawLine(renderer, x - xRadius, y + yOffset, x + xRadius, y + yOffset);

        if (status < 0) {
            status = -1;
            break;
        }
    }

    return status;
}
