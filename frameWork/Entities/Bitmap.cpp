
#include <iostream>
#include <fstream>
#include "Bitmap.h"
#include "../Graphics/Graphics.h"

Bitmap::Bitmap(std::string path, int width, int height)
{
	
	this->width = width;
	this->height = height;
	std::streampos size;
	char* memoryBlock;
	FILE* f;
	//fopen_s(&f, path.c_str(), "rb");
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		//Read in file
		size = file.tellg();
		memoryBlock = new  char[size];
		file.seekg(0, std::ios::beg);
		file.read(memoryBlock, size);
		file.close();

		unsigned char info[54];
		memcpy(info, memoryBlock, 54); // read the 54-byte header
	
		// extract image height and width from header
		int imageWidth = *(int*)&info[18];
		int imageHeight = *(int*)&info[22];
		//Pad the row size to be a rounded up multiple of 4
		int row_padded = (imageWidth * 3 + 3) & (~3);
		unsigned char* data = new unsigned char[row_padded];
		unsigned char tmp;
		
		float wRatio = (float)width / (float)imageWidth;
		float hRatio = (float)height / (float)imageHeight;		
		
		//Allocate the required memory for the pixels
		pixels = new Color*[width];
		for (int i = 0; i < width; i++)
		{
			pixels[i] = new Color[height];
		}
		
		//Load the image into memory
		Color** image = new Color*[row_padded/3];
		for (int i = 0; i < imageWidth; i++)
		{
			image[i] = new Color[imageHeight];
		}

		for (int i = 0; i < imageHeight; i++)
		{
			memcpy(data,54 + memoryBlock +i*row_padded, row_padded);
			for (int j = 0; j < row_padded-1; j+=3)
			{
				image[(int)(j/3)][i] = Color(Color(data[j + 2], data[j + 1], data[j]));

			}

		}	

		delete[] data;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int imageHorIndex = (int)(0.0f + ((float)i/ wRatio));
				int imageVerIndex = (int)(0.0f + ((float)j /hRatio));
				pixels[i][j] = image[imageHorIndex][imageHeight - 1 - imageVerIndex];

			}
		}
		for (int i = 0; i < imageWidth; i++)
		{
			delete[] image[i];
		}	
		delete[] image;

		
	}
	
}
Bitmap::~Bitmap()
{
	for (int i = 0; i < width; i++)
	{
		delete[] pixels[i];
	}
	delete[] pixels;
}