#pragma once
#include <vector>
#include <string>
class Color;
class Bitmap {
public:

	Bitmap(std::string path, int width, int height);
	~Bitmap();
	Color** pixels;
	int GetWidth() { return width;}
	int GetHeight() { return height; };
private:

	int width;
	int height;





};