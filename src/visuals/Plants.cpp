#include "visuals/Plants.h"

Branch::Branch(vec2d pose, float thickness) : pose(pose)
{
	this->thickness = thickness;
	if (this->thickness < 1) this->thickness = 1;
	color = rand()%1000 * 0.001;
}

void Branch::populate(unsigned int child_min, unsigned int child_max, float child_len)
{
	if (child_min > child_max) return;
	unsigned int children;
	if (child_min == child_max)
		children = rand()%child_max;
	else
		children = child_min+rand()%(child_max-child_min);

	for (int i = 0; i < children; i++) {
		int rotation = rand()%30;
		if (i%2 == 0) rotation = -rotation-15;
		else rotation = rotation+15;
		branches.push_back(Branch(vec2d(0, 1).rotate(rotation)*child_len, thickness*.7));
		branches[branches.size()-1].populate(child_min-1, child_max-1, child_len*.7);
	}
}

TreeVisual::TreeVisual(vec2d pos) : pos(pos), root(vec2d(0, 1)*100, 20)
{
	root.populate(2, 5, 100);
	original_size = abs(getMaxX(root, pos)-getMinX(root, pos));
}

void TreeVisual::update()
{
	// Drop seeds
}

void TreeVisual::draw(SDL_Renderer* renderer)
{
	draw_branch(root, pos, renderer);
}

void TreeVisual::draw_branch(Branch branch, vec2d spos, SDL_Renderer* renderer)
{
	vec2d epos(spos.x - branch.pose.x*scale, spos.y - branch.pose.y*scale);
	thickLineRGBA(renderer,
    	spos.x, spos.y,
    	epos.x, epos.y,
    	branch.thickness*scale,
    	150*branch.color, 90*branch.color, 70*branch.color, 255);
	for (auto b : branch.branches)
		draw_branch(b, epos, renderer);
}

float TreeVisual::getMinX(Branch branch, vec2d spos)
{
	vec2d epos(spos.x - branch.pose.x, spos.y - branch.pose.y);
	float winner = epos.x;

	for (auto b : branch.branches) {
		float childw = getMinX(b, epos);
		if (childw < winner) winner = childw;
	}

	return winner;
}

float TreeVisual::getMaxX(Branch branch, vec2d spos)
{
	vec2d epos(spos.x - branch.pose.x, spos.y - branch.pose.y);
	float winner = epos.x;

	for (auto b : branch.branches) {
		float childw = getMaxX(b, epos);
		if (childw > winner) winner = childw;
	}

	return winner;
}

void TreeVisual::setScale(float new_size)
{
	scale = new_size/original_size;
}

void TreeVisual::setPos(vec2d pos)
{
	this->pos = pos;
}
