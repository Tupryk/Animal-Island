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

Limb::Limb(vec2d* base, unsigned int seg_count, float seg_len) : seg_count(seg_count) {
	this->base = base;
    for (int i = 0; i < seg_count; i++) {
   		Segment seg(0, 0, seg_len, 0);
        segs.push_back(seg);
	}
}

void Limb::update(vec2d follows)
{
	segs[0].follow(follows);
    segs[0].update();

    for (int i = 1; i < seg_count; i++) {
        segs[i].follow(segs[i-1].origin);
        segs[i].update();
    }

    segs[seg_count-1].set_origin(*base);
    for (int i = seg_count-2; i > -1; i--)
        segs[i].set_origin(segs[i+1].end);
}

AnimalVisual::AnimalVisual() :
	pos(300, 200),
	limbs{
		Limb(&body_joints[0], 2, 70),
		Limb(&body_joints[1], 2, 70),
		Limb(&body_joints[2], 2, 70),
		Limb(&body_joints[3], 2, 70)
}{
    body_joints[0] = vec2d(-100, 40);
    body_joints[1] = vec2d(-80, 30);
    body_joints[2] = vec2d(100, 40);
    body_joints[3] = vec2d(120, 30);
}


void AnimalVisual::update()
{
	leg_target += speed;
	if (leg_target >= 110 || leg_target <= -100) speed *= -1;

	visual_coors.x = leg_target;
	visual_coors.y = 100;
	for (int i = 0; i < limb_count; i++)
		limbs[i].update({visual_coors.x, visual_coors.y});
}

void AnimalVisual::draw(SDL_Renderer* renderer)
{
	for (int i = 0; i < limb_count; i++) {
		for (Segment seg : limbs[i].segs) {
		    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		    SDL_RenderDrawLine(renderer,
		    	seg.origin.x + pos.x,
		    	seg.origin.y + pos.y,
		    	seg.end.x + pos.x,
		    	seg.end.y + pos.y);
		}
	}
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillCircle(renderer, visual_coors.x + pos.x, visual_coors.y + pos.y, 10);
}
