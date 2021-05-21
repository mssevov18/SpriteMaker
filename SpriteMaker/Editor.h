#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "windows.h"
#include "conio.h"

#include "Sprite.h"

#define pallete palette
#define backgroundColour palette[0]
#define primaryColour palette[1]
#define secondaryColour palette[2]
#define pointerColour palette[3]

Position getResolution();

//create properties menu
//create pointer shape

class Editor
{
public:
	Editor();
	~Editor();

	void mainLoop();

protected:
	HDC hdc;
	Position resolution;

	Position pointer;
	Position pastPointer;
	unsigned brushSize;
	std::vector<COLOURREF> palette;

	Sprite activeSprite;



protected:
	void propertiesEditor();

	void showPalette(Position rootPos);

	void updateSprite(char mode);

	void saveSprite(std::string path);
	void loadSprite(std::string path);

};

