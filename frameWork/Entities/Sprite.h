#pragma once
#include <vector>
#include <string>
#include "Bitmap.h"
class Graphics;
class Canvas;

class Sprite {
public:
	Sprite(std::string path,int width,int height);
//	Sprite(std::string path, Graphics* gfx);
	~Sprite();
	void Draw(int x, int y, int depth, Canvas* C);
	void Draw(int x, int y, Graphics* gfx);


private:
	int width;
	int height;
	int centerX;
	int centerY;
	std::vector<Bitmap> myFrames;
	Bitmap* myFrame;




};