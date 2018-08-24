#include <thread>
#include <ctime>
#include "GameIncludes.h"
const int numObjects = 50;
const int yCutoff = 800;
Game::Game(HWND* hWnd, Keyboard* kbd, Mouse* ms)
{
	pMyWindow = hWnd;
	drawingColor = new Color(255, 0, 0);
	myKbd = kbd;
	myMouse = ms;
	this->SubscribeKeyboard(kbd);
	this->SubscribeMouse(ms);
	SCREEN_WIDTH = SCREEN_WIDTH_START;
	SCREEN_HEIGHT = SCREEN_HEIGHT_START;
	pMyGfx = new Graphics(*hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);


	otest1 = new oTest*[numObjects];
	cBackGround = new Canvas(SCREEN_WIDTH, SCREEN_HEIGHT, pMyGfx);
	cBackGround->SetBackGroundColor(Color(0, 0, 255));
	cTest = new Canvas(SCREEN_WIDTH/4, SCREEN_HEIGHT/4, pMyGfx);

	cTest->SetPos(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
	cTest->SetBackGroundColor(Color(255, 0, 0));
	cTest->horizontalWrapping = true;
	cTest->verticalWrapping = true;

	cBackGround->horizontalWrapping = true;
	cBackGround->verticalWrapping = true;
	for (int i = 0; i < numObjects; i++)
	{
		otest1[i] = new oTest(this, cTest, 10, 10);
		otest1[i]->x = i*cTest->GetWidth() / numObjects;
		otest1[i]->y = 200;
		otest1[i]->depth = i;
	}
}
Game::~Game()
{
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
		cTest->SetPos(cTest->GetX() + speed*(calcPeriod / 1000.0f), cTest->GetY());
	}
	else if (myKbd->GetKeyState('A') == true)
	{
		cTest->SetPos(cTest->GetX() - speed*(calcPeriod / 1000.0f), cTest->GetY());
	}
	if (myKbd->GetKeyState('W') == true)
	{
		cTest->SetPos(cTest->GetX() ,cTest->GetY() - speed*(calcPeriod / 1000.0f));
	}
	if (myKbd->GetKeyState('S') == true)
	{
		cTest->SetPos(cTest->GetX(), cTest->GetY() + speed*(calcPeriod / 1000.0f));
	}
	for (int i = 0; i < numObjects; i++)
	{
		otest1[i]->Action();
		otest1[i]->x+= 10*(numObjects - i)/numObjects;
		if (otest1[i]->x > cTest->GetWidth())
		{
			otest1[i]->x = 0;
		}
		if (otest1[i]->x< 0)
		{
			otest1[i]->x = cTest->GetWidth();
		}
		if (otest1[i]->y > cTest->GetHeight())
		{
			otest1[i]->y = 0;
		}
		if (otest1[i]->y< 0)
		{
			otest1[i]->y = cTest->GetHeight();
		}

	}
}
void Game::DrawScreen()
{
	if (seperateDrawingFromActions == false)
	{
		Actions();
	}

	pMyGfx->BeginFrame();
	/*
	for (int i = 0; i < SCREEN_WIDTH / 100; i++)
	{
		pMyGfx->DrawLine(100 * i, 0, 100 * i, SCREEN_HEIGHT, *drawingColor);
	}
	for (int i = 0; i < SCREEN_HEIGHT / 100; i++)
	{
		pMyGfx->DrawLine(0, 100 * i, SCREEN_WIDTH, 100 * i, *drawingColor);
	}
	*/
	//pMyGfx->DrawCircle((int)x, (int)y, 30, *drawingColor);
	
	cBackGround->ClearCanvas();
	for (int i = 0; i < numObjects; i++)
	{
		otest1[i]->Draw();
	}
	cBackGround->DrawToCanvas(cTest->GetX() , cTest->GetY(), cTest);
	cBackGround->DrawToCanvas(cTest->GetX() + 400, cTest->GetY() + 200, cTest);
	cBackGround->DrawCavas();
	pMyGfx->EndFrame();
}