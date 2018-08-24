#include "Graphics.h"
void Graphics::DrawPixel(int x, int y, int r, int g, int b)
{
	DrawPixel(x, y, Color(r, g, b));
}
void  Graphics::DrawPixel(int x, int y, Color color)
{
	if ((x < 0 || x >= width) && wrapHorizontal == true)
	{
		x = abs(x % width);
	}
	if ((y < 0 || y >= height) && wrapVertical == true)
	{
		y = abs(y % height);
	}
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		long location = y*width + x;
		pixelBuffer[location] = color;
	}
}
//Draw a line using Bresenhams algoritm
void Graphics::DrawLine(int x1, int y1, int x2, int y2, Color c)
{
	int dx = abs(x2 - x1), sx = x1<x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1<y2 ? 1 : -1;
	int err = (dx>dy ? dx : -dy) / 2, e2;

	for (;;) {
		DrawPixel(x1, y1, c);
		if (x1 == x2 && y1 == y2) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x1 += sx; }
		if (e2 < dy) { err += dx; y1 += sy; }
	}
}

void Graphics::FillCircle(int xCenter, int yCenter, int radius, Color c)
{
	int xStart = xCenter - radius;
	int yStart = yCenter - radius;

	int radSquared = radius*radius;
	for (int i = -radius; i < radius; i++)
	{
		for (int j = -radius; j < radius; j++)
		{
			if (i*i + j*j < radSquared)
			{
				DrawPixel(i + xStart + radius, j + yStart + radius, c);
			}
		}
	}


}
//Draw a circle using Bresenhams algoritm
void Graphics::DrawCircle(int xCenter, int yCenter, int radius, Color c)
{
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		DrawPixel(xCenter + x, yCenter + y, c);
		DrawPixel(xCenter + y, yCenter + x, c);
		DrawPixel(xCenter - y, yCenter + x, c);
		DrawPixel(xCenter - x, yCenter + y, c);
		DrawPixel(xCenter - x, yCenter - y, c);
		DrawPixel(xCenter - y, yCenter - x, c);
		DrawPixel(xCenter + y, yCenter - x, c);
		DrawPixel(xCenter + x, yCenter - y, c);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}
void Graphics::FillRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c)
{
	if (width < 0)
	{
		width = -width;
		xTopLeft -= width;
	}
	if (height < 0)
	{
		height = -height;
		yTopLeft -= height;
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			DrawPixel(xTopLeft + i, yTopLeft + j, c);
		}
	}
}
void Graphics::DrawRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c)
{

	DrawLine(xTopLeft, yTopLeft, xTopLeft + width, yTopLeft, c);
	DrawLine(xTopLeft + width, yTopLeft, xTopLeft + width, yTopLeft + height, c);
	DrawLine(xTopLeft + width, yTopLeft + height, xTopLeft, yTopLeft + height, c);
	DrawLine(xTopLeft, yTopLeft + height, xTopLeft, yTopLeft, c);
}


//from internet, should look into more
void Graphics::DrawBMP(int x, int y, int drawWidth, int drawHeight, std::string fileName)
{
	FILE* f;
	fopen_s(&f, fileName.c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;
	float wRatio = (float)drawWidth / (float)width;
	float hRatio = (float)drawHeight / (float)height;
	int size = 3 * width * height;
	for (int i = 0; i < height; i++)
	{

		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;
			for (int k = 0; k < wRatio; k++)
			{
				for (int l = 0; l < wRatio; l++)
				{
					DrawPixel((int)(x + k + ((j / 3)*wRatio)), (int)(y + l + ((height - i)*hRatio)), Color((int)data[j], (int)data[j + 1], (int)data[j + 2]));
				}
			}
		}
	}

	fclose(f);
}

// this is the function that cleans up Direct3D and COM
Graphics::~Graphics(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	if (pixelBuffer)
	{
		_aligned_free(pixelBuffer);
		pixelBuffer = nullptr;
	}

	if (gfxDeviceCon != NULL)
	{
		gfxDeviceCon->ClearState();
	}
}
