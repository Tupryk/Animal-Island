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

int SDL_RenderFillAlmond(SDL_Renderer* renderer, vec2d pos, vec2d origin0, vec2d origin1, float radius0, float radius1, int r, int g, int b)
{
    const unsigned int s_count = 8;
    unsigned int vert_counter = 0;
    static const unsigned int total_verts = s_count*2 + 2;
    Sint16 vertsx[total_verts];
    Sint16 vertsy[total_verts];

    // Calculate circle intersections
    float dx = origin1.x - origin0.x;
    float dy = origin1.y - origin0.y;
    float d = sqrt(dx * dx + dy * dy);

    // Circles fo not intersect
    if (d > radius0 + radius1 || d < abs(radius0 - radius1))
        return -1;

    float a = (radius0 * radius0 - radius1 * radius1 + d * d) / (2 * d);
    float h = sqrt(radius0 * radius0 - a * a);
    
    float intersectionX = origin0.x + a * (origin1.x - origin0.x) / d;
    float intersectionY = origin0.y + a * (origin1.y - origin0.y) / d;
    
    vec2d inter0(intersectionX + h * (origin1.y - origin0.y) / d, intersectionY - h * (origin1.x - origin0.x) / d);
    vec2d inter1(intersectionX - h * (origin1.y - origin0.y) / d, intersectionY + h * (origin1.x - origin0.x) / d);

    inter0 = inter0 + pos;
    inter1 = inter1 + pos;

    vertsx[vert_counter] = static_cast<int>(inter1.x);
    vertsy[vert_counter] = static_cast<int>(inter1.y);
    vert_counter++;

    float tmp = (inter0-inter1).get_length();
    vec2d normed = (inter0-inter1).norm();
    const float s_len = tmp/(s_count+1);
    for (int i = 1; i < s_count+1; i++)
    {
        float x = s_len*i - tmp*.5;
        vec2d x_vec = normed*x;
        float y_up = -(sqrt(radius1*radius1-x*x)-origin1.y);
        vec2d up = x_vec + normed.rotate(90)*y_up;
        up = up + pos;

        vertsx[vert_counter] = static_cast<int>(up.x);
        vertsy[vert_counter] = static_cast<int>(up.y);
        vert_counter++;
    }
    vertsx[vert_counter] = static_cast<int>(inter0.x);
    vertsy[vert_counter] = static_cast<int>(inter0.y);
    vert_counter++;

    for (int i = s_count; i >= 1; i--)
    {
        float x = s_len*i - tmp*.5;
        vec2d x_vec = normed*x;
        float y_down = sqrt(radius0*radius0-x*x)+origin0.y;
        vec2d down = x_vec + normed.rotate(90)*y_down;
        down = down + pos;

        vertsx[vert_counter] = static_cast<int>(down.x);
        vertsy[vert_counter] = static_cast<int>(down.y);
        vert_counter++;
    }

    // draw
    filledPolygonRGBA(renderer, vertsx, vertsy, vert_counter, r, g, b, 255);
    return 0;
}

int SDL_RenderFillMoon(SDL_Renderer* renderer, vec2d pos, vec2d origin0, vec2d origin1, float radius0, float radius1, int r, int g, int b)
{
    const unsigned int s_count = 8;
    unsigned int vert_counter = 0;
    static const unsigned int total_verts = s_count*2 + 2;
    Sint16 vertsx[total_verts];
    Sint16 vertsy[total_verts];

    // Calculate circle intersections
    float dx = origin1.x - origin0.x;
    float dy = origin1.y - origin0.y;
    float d = sqrt(dx * dx + dy * dy);

    // Circles fo not intersect
    if (d > radius0 + radius1 || d < abs(radius0 - radius1))
        return -1;

    float a = (radius0 * radius0 - radius1 * radius1 + d * d) / (2 * d);
    float h = sqrt(radius0 * radius0 - a * a);
    
    float intersectionX = origin0.x + a * (origin1.x - origin0.x) / d;
    float intersectionY = origin0.y + a * (origin1.y - origin0.y) / d;
    
    vec2d inter0(intersectionX + h * (origin1.y - origin0.y) / d, intersectionY - h * (origin1.x - origin0.x) / d);
    vec2d inter1(intersectionX - h * (origin1.y - origin0.y) / d, intersectionY + h * (origin1.x - origin0.x) / d);

    inter0 = inter0 + pos;
    inter1 = inter1 + pos;

    vertsx[vert_counter] = static_cast<int>(inter1.x);
    vertsy[vert_counter] = static_cast<int>(inter1.y);
    vert_counter++;

    float inter_len = (inter0-inter1).get_length();
    vec2d normed = (origin0-origin1).norm();
    vec2d normed_rot = normed.rotate(90);
    const float s_len = inter_len/(s_count+1);
    for (int i = 1; i < s_count+1; i++)
    {
        float x = s_len*i - inter_len*.5;
        float y_up = -(sqrt(radius1*radius1-x*x) + origin1.get_length());
        vec2d up = normed_rot*x + normed*y_up;
        up = up + pos;

        vertsx[vert_counter] = static_cast<int>(up.x);
        vertsy[vert_counter] = static_cast<int>(up.y);
        vert_counter++;

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillCircle(renderer, up.x, up.y, 3);
    }
    vertsx[vert_counter] = static_cast<int>(inter0.x);
    vertsy[vert_counter] = static_cast<int>(inter0.y);
    vert_counter++;

    for (int i = s_count; i >= 1; i--)
    {
        float x = s_len*i - inter_len*.5;
        float y_down = -(sqrt(radius0*radius0-x*x) + origin0.get_length());
        vec2d down = normed_rot*x + normed*y_down;
        down = down + pos;

        vertsx[vert_counter] = static_cast<int>(down.x);
        vertsy[vert_counter] = static_cast<int>(down.y);
        vert_counter++;

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillCircle(renderer, down.x, down.y, 3);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillCircle(renderer, inter0.x, inter0.y, 3);
    SDL_RenderFillCircle(renderer, inter1.x, inter1.y, 3);
    // draw
    //filledPolygonRGBA(renderer, vertsx, vertsy, vert_counter, r, g, b, 255);
    return 0;
}

float crossProduct(vec2d v1, vec2d v2) {
    return (v1.x*v2.y) - (v1.y*v2.x);
}
