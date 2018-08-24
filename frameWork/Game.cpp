#include <thread>
#include <ctime>
#include "oBase.h"
#include "oTest.h" 
#include "game.h"
Game::Game(HWND* hWnd, Keyboard* kbd, Mouse* ms)
{
	pMyWindow = hWnd;
	
	otest1 = new oTest(this);
	myKbd = kbd;
	myMouse = ms;
	this->SubscribeKeyboard(kbd);
	this->SubscribeMouse(ms);
	SCREEN_WIDTH = SCREEN_WIDTH_START;
	SCREEN_HEIGHT = SCREEN_HEIGHT_START;
	pMyGfx = new Graphics(*hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
}
Game::~Game()
{
	delete(drawingColor);
	delete(pMyGfx);

}
void Game::SetScreen(int width, int height, std::string title)
{
	delete(pMyGfx);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	pMyWindow = ChangeWindowMethod(width, height, title);
	pMyGfx = new Graphics(*pMyWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void Game::Start()
{
	if (seperateDrawingFromActions == true)
	{
		while (closing == false)
		{
			std::clock_t begin = clock();
			Actions();
			std::clock_t end;
			while ((end = clock()) - begin < calcPeriod);
		}
	}
}
void Game::Actions()
{
	if (myKbd->GetKeyState('D') == true)
	{
		x += speed*(calcPeriod / 1000.0f);
	}
	else if (myKbd->GetKeyState('A') == true)
	{
		x -= speed*(calcPeriod / 1000.0f);
	}
	if (myKbd->GetKeyState('W') == true)
	{
		y -= speed*(calcPeriod / 1000.0f);
	}
	if (myKbd->GetKeyState('S') == true)
	{
		y += speed*(calcPeriod / 1000.0f);
	}

	otest1->Action();

}
void Game::DrawScreen()
{
	if (seperateDrawingFromActions == false)
	{
		Actions();
	}

	pMyGfx->BeginFrame();

	for (int i = 0; i < SCREEN_WIDTH / 100; i++)
	{
		pMyGfx->DrawLine(100 * i, 0, 100 * i, SCREEN_HEIGHT, *drawingColor);
	}
	for (int i = 0; i < SCREEN_HEIGHT / 100; i++)
	{
		pMyGfx->DrawLine(0, 100 * i, SCREEN_WIDTH, 100 * i, *drawingColor);
	}
	pMyGfx->DrawCircle((int)x, (int)y, 30, *drawingColor);
	otest1->Draw(pMyGfx);

	pMyGfx->EndFrame();
}