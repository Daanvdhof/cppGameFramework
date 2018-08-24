#include "../Graphics/Graphics.h"
#include "../Graphics/Canvas.h"
#include "Sprite.h"

Sprite::Sprite(std::string path, int width, int height)
{
	this->width = width;
	this->height = height;
	centerX = 0;
	centerY = 0;
	myFrame  = new Bitmap(path, width, height);
}
Sprite::~Sprite()
{
	delete(myFrame);
}
void Sprite::Draw(int x, int y, Graphics* gfx)
{
	if (gfx != NULL)
	{
		int startX = x + centerX;
		int startY = y + centerY;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				gfx->DrawPixel(startX + i, startY + j, myFrame->pixels[i][j]);
			}
		}
	}
}
void Sprite::Draw(int x, int y, int depth, Canvas* canvas)
{
	if (canvas != NULL)
	{
		int startX = x + centerX;
		int startY = y + centerY;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				canvas->DrawToCanvas(startX + i, startY + j, depth, myFrame->pixels[i][j]);
			}
		}
	}
}