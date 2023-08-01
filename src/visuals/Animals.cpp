#include "visuals/Animals.h"

Segment::Segment(float x, float y, float length, float angle) :
	origin(x, y), end(0, 0), length(length), angle(angle) {
}

void Segment::follow(vec2d target) {
	vec2d dir = target - origin;
	angle = atan2(dir.y, dir.x);
	dir = (dir * length / dir.get_length()) * -1.;
	origin = target + dir;
}

void Segment::set_origin(vec2d pos) {
	origin = pos;
	update();
}

void Segment::update() {
	vec2d d(length * cos(angle), length * sin(angle));
    end = origin + d;
}

Limb::Limb(unsigned int seg_count, float seg_len, vec2d base) :
	seg_count(seg_count), base(base) {
    for (int i = 0; i < seg_count; i++) {
   		Segment seg(300, 200, seg_len, 0);
        segs.push_back(seg);
	}
}

void Limb::update(vec2d follows) {
	segs[0].follow(follows);
    segs[0].update();

    for (int i = 1; i < seg_count; i++) {
        segs[i].follow(segs[i-1].origin);
        segs[i].update();
    }

    segs[seg_count-1].set_origin(base);

    for (int i = seg_count-2; i > -1; i--)
        segs[i].set_origin(segs[i+1].end);
}

AnimalVisual::AnimalVisual()
{
	Limb limb;
	limbs.push_back(limb);
}

void AnimalVisual::update()
{
	leg_target += speed;
	if (leg_target >= 500 || leg_target <= 100) speed *= -1;

	visual_coors.x = cos( (-.5 * (leg_target-100)/400) * M_PI ) * 400 + 100;
	visual_coors.y = 300;
	for (int i = 0; i < limbs.size(); i++)
		limbs[i].update({visual_coors.x, visual_coors.y});
}

void AnimalVisual::draw(SDL_Renderer* renderer)
{
	for (Limb limb : limbs) {
		for (Segment seg : limb.segs) {
		    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		    SDL_RenderDrawLine(renderer, seg.origin.x, seg.origin.y, seg.end.x, seg.end.y);
		}
	}
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillCircle(renderer, visual_coors.x, visual_coors.y, 10);
}
