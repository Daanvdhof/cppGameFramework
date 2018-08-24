#pragma once
#include <limits> 
class Color;
class Graphics;


class Canvas
{
public:
	Canvas(int width, int height, Graphics* gfx);
	~Canvas();
	void DrawToCanvas(int x, int y,int depth, Color c);
	void DrawToCanvasWrapped(int x, int y, int depth, Color c);

	void DrawToCanvas(int x, int y, Canvas* canvas);
	void DrawToCanvasWrapped(int x, int y, Canvas* canvas);

	void DrawCavas();
	void SetPos(int x, int y) { this->x = x; this->y = y; }
	int GetX() { return x;}
	int GetY() { return y;}
	int GetWidth(){ return canvasWidth;}
	int GetHeight() { return canvasHeight; }
	void Resize(int newWidth, int newHeight);
	void ClearCanvas() { ClearRegion(0, 0, canvasWidth, canvasHeight);}
	void ClearRegion(int x, int y, int width, int height);
	void SetBackGroundColor(Color c) { backGroundColor = c;}
	int GetDepth(int x, int y) { if (x >= 0 && y >= 0 && x < canvasWidth && y < canvasHeight) { return minPixelDepth[x][y]; } else { return 127;}}
	Color GetPixel(int x, int y) { if (x >= 0 && y >= 0 && x < canvasWidth && y < canvasHeight) { return pixels[x][y]; } else { return backGroundColor; } }

private:
	int canvasWidth;
	int canvasHeight;
	int x;
	int y;
	int** minPixelDepth;
	Color** pixels;
	Graphics* myGfx;
	Color backGroundColor;
	const int maxDepth = 255;

};