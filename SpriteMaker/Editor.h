#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "windows.h"
#include "conio.h"

#include "Sprite.h"

class Editor
{
public:
	Editor();
	~Editor();

	void mainLoop();

protected:
	HWND      hwnd;
	HDC       hdc;

	Position pointer;
	Position pastPointer;
	unsigned pixelScale;

	COLOURREF mainColour;
	COLOURREF secondaryColour;
	COLOURREF backgroundColour;

	Sprite activeSprite;



protected:

	void spriteEditor();
	void propertiesEditor();

	void updateSprite(char mode);

	void saveSprite(std::string path);
	void loadSprite(std::string path);

};

