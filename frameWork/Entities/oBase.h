#pragma once
#include "ForwardDeclarations.h"

class oBase
{
public:
	oBase(Game* game, Graphics* gfx);
	oBase(Game* game, Canvas* canvas);
	virtual void Action() = 0;
	float x;
	float y;
	virtual void Draw() = 0;
protected:

	//Sprite mySprite;

	Game* thisGame;
	Sprite* mySprite;
	Graphics* gfx;
	Canvas* myCanvas;
};