#include "Includes.h"
#include "oTest.h"

oTest::oTest(Game* game, Graphics* gfx, int width, int height) : oBase(game, gfx)
{
	mySprite = new Sprite("Images/dis.bmp", width, height);
	depth = 0;
}
oTest::oTest(Game* game, Canvas* canvas, int width, int height) : oBase(game, canvas)
{
	mySprite = new Sprite("Images/dis.bmp", width, height);
	depth = 0;
}
oTest::~oTest()
{
	if (mySprite != NULL)
	{
		delete(mySprite);
	}
}

void oTest::Action()
{
	if (thisGame->myKbd->GetKeyState(KEY_LEFT))
	{
		x -= 1;
	}
	else if (thisGame->myKbd->GetKeyState(KEY_RIGHT))
	{
		x += 1;
	}
	if (thisGame->myKbd->GetKeyState(KEY_UP))
	{
		y -= 1;
	}
	else if (thisGame->myKbd->GetKeyState(KEY_DOWN))
	{
		y += 1;
	}
}
void oTest::Draw()
{
	if (gfx != NULL)
	{
		if (mySprite != NULL)
		{
			mySprite->Draw((int)x, (int)y, gfx);
		}
	}
	else if (myCanvas != NULL)
	{
		if (mySprite != NULL)
		{
			mySprite->Draw((int)x, (int)y,depth,  myCanvas);
		}
	}
}

