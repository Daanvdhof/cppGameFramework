#include "oBase.h"
#include "Game.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "oTest.h"

oTest::oTest(Game* game) : oBase(game)
{
	thisGame = game;
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
void oTest::Draw(Graphics* gfx)
{
	if (gfx != NULL)
	{
		gfx->DrawCircle(x, y, 20, Color(255, 0, 0));
	}
}

