#include "Sprite.h"


Position::Position()
{
	x = 0;
	y = 0;
}

Position::Position(int newX, int newY)
{
	x = newX;
	y = newY;
}

bool operator==(Position& l, Position& r)
{
	return (l.x == r.x) & (l.y == r.y);
}

bool operator!=(Position& l, Position& r)
{
	return !(l == r);
}


Pixel::Pixel()
{
	colour = RGB(0, 0, 0);
}

Pixel::Pixel(Position newPos, COLOURREF newColour)
{
	pos = newPos;
	colour = newColour;
}

void Pixel::updatePixel(int addX, int addY)
{
	pos.x += addX;
	pos.y += addY;
}

bool Pixel::operator== (Pixel& r)
{
	return (this->pos == r.pos) && (this->colour == r.colour);
}

bool Pixel::operator!= (Pixel& r)
{
	return !(*this == r);
}


Sprite::Sprite()
{
	vPixel.clear();
}

Sprite::~Sprite()
{
	vPixel.clear();
}

Sprite Sprite::rectangle(Position rootPos, unsigned size, COLOURREF colour)
{
	Sprite out;
	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
			out.addPixel(Pixel(
				Position(
					int(j + rootPos.x),
					int(i + rootPos.y)),
				colour)
			);
	return out;
}

Sprite Sprite::rectangle(Position rootPos, Position endPos, COLOURREF colour)
{
	if (rootPos.x >= endPos.x or rootPos.y >= endPos.y)
		return Sprite();

	Sprite out;
	for (int i = 0; i < endPos.y; i++)
		for (int j = 0; j < endPos.x; j++)
			out.addPixel(Pixel(
				Position(
					j + rootPos.x,
					i + rootPos.y),
				colour)
			);
	return out;
}

Sprite Sprite::pointer(Position rootPos, unsigned size, COLOURREF colour)
{
	Sprite out;
	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
		{
			if (
				i == 0 or i == size - 1 or
				j == 0 or j == size - 1 or
				
				size % 2 == 0 and
				(size / 2 == i + 1 and size / 2 == j + 1 or
				 size / 2 == i + 1 and size / 2 == j or
				 size / 2 == i and size / 2 == j + 1 or
				 size / 2 == i and size / 2 == j) or
				
				size % 2 == 1 and
				(size / 2 == i and size / 2 == j or
				 size / 2 == i + 1 and size / 2 == j or
				 size / 2 == i - 1 and size / 2 == j or
				 size / 2 == i and size / 2 == j + 1 or
				 size / 2 == i and size / 2 == j - 1)
				)
				out.addPixel(Pixel(
					Position(
						int(j + rootPos.x),
						int(i + rootPos.y)),
					colour)
				);

		}
	return out;
}

Sprite Sprite::border(Position rootPos, unsigned size, COLOURREF colour)
{
	Sprite out;
	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
		{
			if (
				i == 0 or i == size - 1 or
				j == 0 or j == size - 1
				)
				out.addPixel(Pixel(
					Position(
						int(j + rootPos.x),
						int(i + rootPos.y)),
					colour)
				);

		}
	return out;
}

Pixel Sprite::getPixel(size_t index)
{
	if (index >= vPixel.size())
		return Pixel();
	return vPixel[index];
}

void Sprite::addPixel(Pixel newPixel)
{
	for (size_t i = 0; i < vPixel.size(); i++)
	{
		if (vPixel[i].pos == newPixel.pos)
		{
			vPixel[i] = newPixel;
			return;
		}
	}
	vPixel.push_back(newPixel);
}

void Sprite::removePixel(size_t index)
{
	if (index >= vPixel.size())
		return;
	vPixel.erase(vPixel.begin() + index);
}

//if mode is 0 -> remove by Pixel
//if mode is 1 -> remove by X&Yposition
//if mode is 2 -> remove by Xposition
//if mode is 3 -> remove by Yposition
//if mode is 4 -> remove by COLOURREF
void Sprite::removePixel(Pixel filter, char mode)
{
	switch (mode)
	{
	case 0: // by Pixel
		for (size_t i = 0; i < vPixel.size(); i++)
			if (vPixel[i] == filter)
				vPixel.erase(vPixel.begin() + i);
		return;
	case 1: // by X&Yposition
		for (size_t i = 0; i < vPixel.size(); i++)
			if (vPixel[i].pos.x == filter.pos.x and vPixel[i].pos.y == filter.pos.y)
				vPixel.erase(vPixel.begin() + i);
		return;
	case 2: // by Xposition
		for (size_t i = 0; i < vPixel.size(); i++)
			if (vPixel[i].pos.x == filter.pos.x)
				vPixel.erase(vPixel.begin() + i);
		return;
	case 3: // by Yposition
		for (size_t i = 0; i < vPixel.size(); i++)
			if (vPixel[i].pos.y == filter.pos.y)
				vPixel.erase(vPixel.begin() + i);
		return;
	case 4: // by COLOURREF
		for (size_t i = 0; i < vPixel.size(); i++)
			if (vPixel[i].colour == filter.colour)
				vPixel.erase(vPixel.begin() + i);
		return;
	default:
		return;
	}
}

void Sprite::movePixels(int addX, int addY)
{
	for (size_t i = 0; i < vPixel.size(); i++)
		vPixel[i].updatePixel(addX, addY);
}

void Sprite::movePixels(Position addPos)
{
	for (size_t i = 0; i < vPixel.size(); i++)
		vPixel[i].updatePixel(addPos.x, addPos.y);
}

void Sprite::massColourChange(COLOURREF newColour)
{
	for (size_t i = 0; i < vPixel.size(); i++)
		vPixel[i].colour = newColour;
}

void Sprite::draw(HDC hdc)
{
	for (size_t i = 0; i < vPixel.size(); i++)
		SetPixelStruct(hdc, vPixel[i]);
}

void Sprite::draw(HDC hdc, COLOURREF newColour)
{
	for (size_t i = 0; i < vPixel.size(); i++)
		SetPixelFill(hdc, vPixel[i], newColour);
}

Sprite& Sprite::operator+=(Sprite& rhs)
{
	bool hasAnEqual = false;

	for (size_t i = 0; i < rhs.vPixel.size(); i++)
		this->addPixel(rhs.vPixel[i]);

	return *this;
}

Sprite& Sprite::operator-=(Sprite& rhs)
{
	for (size_t i = 0; i < rhs.vPixel.size(); i++)
		this->removePixel(rhs.vPixel[i], 1);

	return *this;
}

