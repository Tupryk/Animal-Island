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

Limb::Limb(vec2d* base, unsigned int seg_count, float total_len) : seg_count(seg_count) {
	this->base = base;
	float seg_len = total_len/seg_count;
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
		Limb(&body_joints[0], 3, 120),
		Limb(&body_joints[1], 3, 120),
		Limb(&body_joints[2], 3, 120),
		Limb(&body_joints[3], 3, 120)
	},
	head(&head_origin, 2, 50)
{
	head_origin = vec2d(-100, 0);
    body_joints[0] = vec2d(-100, 40);
    body_joints[1] = vec2d(-80, 30);
    body_joints[2] = vec2d(100, 40);
    body_joints[3] = vec2d(120, 30);

    for (int i = 0; i < limb_count; i++) {
    	if (i % 2 == 0)
    		leg_target[i] = vec2d(body_joints[i].x-(target_limit), 0);
    	else
    		leg_target[i] = vec2d(body_joints[i].x+(target_limit), 0);
    }
}


void AnimalVisual::update()
{
	float elevation = sin(pos.x*.05)*10;
	pos.x -= speed;
	pos.y = elevation + 200;
	head.update(head_origin + vec2d(-50, -elevation));
	for (int i = 0; i < limb_count; i++)
	{
		leg_target[i].y = 170-elevation;
		if (target_reseting[i]) {
			leg_target[i].x -= speed;
			if (leg_target[i].x <= body_joints[i].x-target_limit) {
				leg_target[i].x = body_joints[i].x-target_limit;
				target_reseting[i] = false;
			}
			leg_target[i].y -= sin(M_PI*(leg_target[i].x-body_joints[i].x+target_limit)/(target_limit*2))*40;
		} else {
			leg_target[i].x += speed;
			if (leg_target[i].x - body_joints[i].x > target_limit) target_reseting[i] = true;
		}
		limbs[i].update(leg_target[i]);
	}
}

void AnimalVisual::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < limb_count; i++) {
		for (int j = 0; j < limbs[i].segs.size(); j++) {
			Segment seg = limbs[i].segs[j];
			if (j == 0) {
				vec2d tmp = (seg.end - seg.origin).rotate(90);
				vec2d rec = tmp.norm() * (leg_width*.5);
				vec2d p0 = seg.origin + rec;
				vec2d p1 = seg.origin - rec;
				filledTrigonRGBA(renderer,
					p0.x + 300, p0.y + pos.y, p1.x + 300, p1.y + pos.y,
					seg.end.x + 300, seg.end.y + pos.y,
					0, 0, 0, 255);
			} else {
			    thickLineRGBA(renderer,
			    	seg.origin.x + 300, seg.origin.y + pos.y,
			    	seg.end.x + 300, seg.end.y + pos.y,
			    	leg_width, 0, 0, 0, 255);
    			SDL_RenderFillCircle(renderer, seg.end.x + 300, seg.end.y + pos.y, leg_width*.5-1);
			}
		}
	}
	for (Segment seg : head.segs) {
	    thickLineRGBA(renderer,
	    	seg.origin.x + 300,
	    	seg.origin.y + pos.y,
	    	seg.end.x + 300,
	    	seg.end.y + pos.y,
	    	10, 0, 0, 0, 255);
	}    
    unsigned int width = 200;
    unsigned int height = 50;
    SDL_Rect rect;
	rect.x = 300-width*.5;
	rect.y = pos.y-height*.5;
	rect.w = width;
	rect.h = height;
	SDL_RenderFillRect(renderer, &rect);

	vec2d horigin = head.segs[0].end + vec2d(280, pos.y);
	SDL_RenderFillAlmond(renderer,
		horigin, vec2d(10, -50), vec2d(-10, 50), 80, 80);
}
