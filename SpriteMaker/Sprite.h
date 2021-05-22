#pragma once

#include <iostream>
#include <vector>

#include "windows.h"
#include "conio.h"

#define COLOURREF COLORREF

struct Position
{
	Position();
	Position(int newX, int newY);

	int x;
	int y;

};

bool operator== (Position& l, Position& r);
bool operator!= (Position& l, Position& r);


struct Pixel
{
	Pixel();
	Pixel(Position newPos, COLOURREF newColour);

	Position  pos;
	COLOURREF colour;

	void updatePixel(int addX, int addY);

	bool operator== (Pixel& r);
	bool operator!= (Pixel& r);
};

#define GetPixelPosStruct(hdc, position) (GetPixel(hdc, position.x, position.y))
#define GetPixelPixStruct(hdc, pixel) (GetPixel(hdc, pixel.pos.x, pixel.pos.y))
#define SetPixelStruct(hdc, pixel) (SetPixel(hdc, pixel.pos.x, pixel.pos.y, pixel.colour))
#define SetPixelFill(hdc, pixel, newColour) (SetPixel(hdc, pixel.pos.x, pixel.pos.y, newColour))


class Sprite
{
public:
	Sprite();
	~Sprite();

	static Sprite rectangle(Position rootPos, unsigned size, COLOURREF colour);
	static Sprite rectangle(Position rootPos, Position endPos, COLOURREF colour);

	static Sprite pointer(Position rootPos, unsigned size, COLOURREF colour);

	static Sprite border(Position rootPos, unsigned size, COLOURREF colour);

protected:
	std::vector<Pixel> vPixel;

public:
	Pixel getPixel(size_t index);
	void  addPixel(Pixel newPixel);
	void  removePixel(size_t index);
	void  removePixel(Pixel filter, char mode);

	void movePixels(int addX, int addY);
	void movePixels(Position addPos);
	void massColourChange(COLOURREF newColour);

	void draw(HDC hdc);
	void draw(HDC hdc, COLOURREF newColour);

	Sprite& operator+=(Sprite& rhs);
	Sprite& operator-=(Sprite& rhs);
};
