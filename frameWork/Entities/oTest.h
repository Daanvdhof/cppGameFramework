#pragma once
#include "ForwardDeclarations.h"

class oTest :public oBase
{
public:
	oTest(Game* game, Graphics* gfx, int width, int height);
	oTest(Game* game, Canvas* canvas, int width, int height);
	~oTest();
	void Action() override;
	void Draw() override;
	int depth;
};
