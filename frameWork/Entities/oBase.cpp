#include "includes.h"

oBase::oBase(Game* game, Graphics* gfx)
{
	thisGame = game;
	this->gfx = gfx;
	myCanvas = NULL;
	x = 100;
	y = 100;
}

oBase::oBase(Game* game, Canvas* canvas)
{
	thisGame = game;
	myCanvas = canvas;
	gfx = NULL;
	x = 100;
	y = 100;
}