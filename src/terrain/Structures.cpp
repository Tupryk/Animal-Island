#include "terrain/Structures.h"

House::House(vec2d pos, unsigned int color[3]) {
	this->pos = pos; inner_size = vec2d(100, 100); exit_door = vec2d(50, 50);
	for (int i = 0; i < 3; i++)
		this->color[i] = color[i];
}

void House::enter(std::shared_ptr<StaticBody> body)
{
	if (std::find(bodies_inside.begin(), bodies_inside.end(), body) == bodies_inside.end())
        bodies_inside.push_back(body);
}

void House::exit(std::shared_ptr<StaticBody> body)
{
	auto new_end = std::remove_if(bodies_inside.begin(), bodies_inside.end(),
	    [body](const std::shared_ptr<StaticBody>& element) {
	        return element == body;
	    });
   	bodies_inside.erase(new_end, bodies_inside.end());
}

void House::clear_bodies() { bodies_inside.clear(); }

void House::draw_inside(SDL_Renderer* renderer, vec2d origin)
{
	float ScreenCenterX = 600;
	float ScreenCenterY = 350;

	// Floor
	float x = -origin.x;
	float y = -origin.y;
	float z = getZfromY(y, ScreenCenterY*2);

	vec2d p0(ScreenCenterX+x/z, ScreenCenterY+y/z);
	vec2d p1(ScreenCenterX+(x+inner_size.x)/z, ScreenCenterY+y/z);

	y += inner_size.y;
	z = getZfromY(y, ScreenCenterY*2);
	vec2d p2(ScreenCenterX+(x+inner_size.x)/z, ScreenCenterY+y/z);
	vec2d p3(ScreenCenterX+x/z, ScreenCenterY+y/z);

	Sint16 vertsx[4] = {
		static_cast<Sint16>(p0.x),
		static_cast<Sint16>(p1.x),
		static_cast<Sint16>(p2.x),
		static_cast<Sint16>(p3.x)};
    Sint16 vertsy[4] = {
    	static_cast<Sint16>(p0.y),
    	static_cast<Sint16>(p1.y),
    	static_cast<Sint16>(p2.y),
    	static_cast<Sint16>(p3.y)};

    filledPolygonRGBA(renderer, vertsx, vertsy, 4, 200, 100, 100, 255);

    // Exit
    x = exit_door.x-origin.x;
    y = exit_door.y-origin.y;
    z = getZfromY(y, ScreenCenterY*2);
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillCircle(renderer, ScreenCenterX+x/z, ScreenCenterY+y/z, 10);

    // Origin
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillCircle(renderer, ScreenCenterX, ScreenCenterY, 10);

    // Entities
    for (auto entity : bodies_inside) {
		// if (std::dynamic_pointer_cast<Person>(entity)) {
	    	x = entity->pos.x-origin.x;
		    y = entity->pos.y-origin.y;
		    z = getZfromY(y, ScreenCenterY*2);
			SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
    		SDL_RenderFillCircle(renderer, ScreenCenterX+x/z, ScreenCenterY+y/z, 10);
    	// }
	}
}

void House::draw_outside(SDL_Renderer* renderer, vec2d origin)
{
	float ScreenCenterX = 600;
	float ScreenCenterY = 350;

	float size = 25;

	float tx = pos.x-origin.x-size;
	float ty = pos.y-origin.y;
	float tz = getZfromYcurberd(ty, ScreenCenterY*2);

	vec2d start(tx-size*.5, ty);
	vec2d end(tx+size*.5, ty);
	float visual_size = ((start-end)*tz).get_length();

	float bty = pos.y-origin.y-size;
	float btz = getZfromYcurberd(bty, ScreenCenterY*2);
	{
    	Sint16 vertsx[4] = {
    		static_cast<Sint16>(ScreenCenterX+(tx-visual_size*.5)*tz),
    		static_cast<Sint16>(ScreenCenterX+(tx+visual_size*.5)*tz),
    		static_cast<Sint16>(ScreenCenterX+(tx+visual_size*.5)*btz),
    		static_cast<Sint16>(ScreenCenterX+(tx-visual_size*.5)*btz)};
	    Sint16 vertsy[4] = {
	    	static_cast<Sint16>(ScreenCenterY+(ty-visual_size)*tz),
	    	static_cast<Sint16>(ScreenCenterY+(ty-visual_size)*tz),
	    	static_cast<Sint16>(ScreenCenterY+(bty-visual_size)*btz),
	    	static_cast<Sint16>(ScreenCenterY+(bty-visual_size)*btz)};

	    filledPolygonRGBA(renderer, vertsx, vertsy, 4, color[0], color[1], color[2], 255);
	}

	SDL_Rect rect;
    rect.x = ScreenCenterX+(tx-visual_size*.5)*tz;
    rect.y = ScreenCenterY+(ty-visual_size)*tz;
    rect.w = visual_size*tz;
    rect.h = visual_size*tz;

    SDL_SetRenderDrawColor(renderer, color[0]*.75, color[1]*.75, color[2]*.75, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (rect.x != 0) {
    	if (rect.x > ScreenCenterX) {
    		Sint16 ftx = ScreenCenterX+(tx-visual_size*.5)*tz;
	    	Sint16 btx = ScreenCenterX+(tx-visual_size*.5)*btz;
	    	Sint16 vertsx[4] = { ftx, ftx, btx, btx };
		    Sint16 vertsy[4] = {
		    	static_cast<Sint16>(ScreenCenterY+ty*tz),
		    	static_cast<Sint16>(ScreenCenterY+(ty-visual_size)*tz),
		    	static_cast<Sint16>(ScreenCenterY+(bty-visual_size)*btz),
		    	static_cast<Sint16>(ScreenCenterY+bty*btz)};

		    filledPolygonRGBA(renderer, vertsx, vertsy, 4, color[0]*.5, color[1]*.5, color[2]*.5, 255);
	    }
	    else if (rect.x < ScreenCenterX) {
	    	Sint16 ftx = ScreenCenterX+(tx+visual_size*.5)*tz;
	    	Sint16 btx = ScreenCenterX+(tx+visual_size*.5)*btz;
	    	Sint16 vertsx[4] = { ftx, ftx, btx, btx };
		    Sint16 vertsy[4] = {
		    	static_cast<Sint16>(ScreenCenterY+ty*tz),
		    	static_cast<Sint16>(ScreenCenterY+(ty-visual_size)*tz),
		    	static_cast<Sint16>(ScreenCenterY+(bty-visual_size)*btz),
		    	static_cast<Sint16>(ScreenCenterY+bty*btz)};

		    filledPolygonRGBA(renderer, vertsx, vertsy, 4, color[0]*.5, color[1]*.5, color[2]*.5, 255);
	    }
    }
}
