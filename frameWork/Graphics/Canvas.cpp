#include "Graphics.h"
#include "Canvas.h"

Canvas::Canvas(int width, int height, Graphics* gfx)
{
	myGfx = gfx;
	SetPos(0, 0);
	Resize(width, height);
	SetBackGroundColor(Color(0, 0, 0));
}
Canvas::~Canvas()
{
	for (int i = 0; i < canvasHeight; i++)
	{
		delete[] minPixelDepth[i];
		delete[] pixels[i];
	}
	delete[] minPixelDepth;
	delete[] pixels;

}
void Canvas::DrawToCanvasWrapped(int x, int y, int depth, Color c)
{
	if ((x < 0 || x >= canvasWidth))
	{
		if (x < 0)
		{
			x %= canvasWidth;
			x += canvasWidth;
		}
		else
		{
			x = (x) % canvasWidth;
		}
	}
	if ((y < 0 || y >= canvasHeight))
	{
		if (y < 0)
		{
			y %= canvasHeight;
			y += canvasHeight;
		}
		else
		{
			y = y % canvasHeight;
		}
	}
	DrawToCanvas(x, y, depth, c);
}

void Canvas::DrawToCanvasWrapped(int x, int y, Canvas* canvas)
{
	int canvasWidth = canvas->GetWidth();
	int canvasHeight = canvas->GetHeight();

	for (int i = 0; i < canvas->canvasWidth; i++)
	{
		for (int j = 0; j < canvas->canvasHeight; j++)
		{
			DrawToCanvasWrapped(x + i, y + j, canvas->GetDepth(i, j), canvas->GetPixel(i, j));
		}
	}
}
void Canvas::DrawToCanvas(int x, int y, int depth, Color c)
{	
	if (x >= 0 && y >= 0 && x < canvasWidth && y < canvasHeight)
	{
		int currentDepth = minPixelDepth[x][y];
		if (depth <= currentDepth)
		{
			pixels[x][y] = c;
			minPixelDepth[x][y] = depth;
		}
	}
}
void Canvas::DrawToCanvas(int x, int y, Canvas* canvas)
{
	int canvasWidth = canvas->GetWidth();
	int canvasHeight = canvas->GetHeight();

	for (int i = 0; i < canvas->canvasWidth; i++)
	{
		for (int j = 0; j < canvas->canvasHeight; j++)
		{
			DrawToCanvas(x + i, y + j, canvas->GetDepth(i, j), canvas->GetPixel(i, j));
		}
	}
}

void Canvas::ClearRegion(int x, int y, int width, int height)
{
	for (int i = x; i < x + width; i++)
	{
		for (int j = y; j < y + height; j++)
		{
			if (i >= 0 && j >= 0 && i < canvasWidth && j < canvasHeight)
			{
				pixels[i][j] = backGroundColor;
				minPixelDepth[i][j] = maxDepth;
			}
		}
	}
}
void Canvas::DrawCavas()
{
	if (myGfx != NULL)
	{
		for (int i = 0; i < canvasWidth; i++)
		{
			for (int j = 0; j < canvasHeight; j++)
			{
				myGfx->DrawPixel(x + i, y + j, pixels[i][j]);
			}
		}
	}
}
void Canvas::Resize(int newWidth, int newHeight)
{
	for (int i = 0; i < canvasHeight; i++)
	{
		if (minPixelDepth[i] != NULL)
		{
			delete[] minPixelDepth[i];
		}
		if (pixels[i] != NULL)
		{
			delete[] pixels[i];
		}
	}
	if (minPixelDepth != NULL)
	{
		delete[] minPixelDepth;
	}
	if (pixels != NULL)
	{
		delete[] pixels;
	}
	canvasWidth = newWidth;
	canvasHeight = newHeight;

	minPixelDepth = new int*[canvasWidth];
	pixels = new Color*[canvasWidth];
	for (int i = 0; i < canvasWidth; i++)
	{
		minPixelDepth[i] = new int[canvasHeight];
		pixels[i] = new Color[canvasHeight];
		for (int j = 0; j < canvasHeight; j++)
		{
			minPixelDepth[i][j] = maxDepth;
			pixels[i][j] = backGroundColor;
		}
	}
}