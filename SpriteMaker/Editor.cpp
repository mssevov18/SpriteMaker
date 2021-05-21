#include "Editor.h"

Position getResolution()
{
	RECT desktop;
	const HWND hDesktop = GetConsoleWindow();
	GetWindowRect(hDesktop, &desktop);
	return Position(desktop.right, desktop.bottom);
}


Editor::Editor()
{
	HWND hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);
	resolution = getResolution();
	palette.push_back(GetPixel(hdc, 0, 0));
	palette.push_back(RGB(190, 160, 75));
	palette.push_back(RGB(50, 100, 220));
	palette.push_back(RGB(230, 230, 230));
	pointer.x = 50;
	pointer.y = 50;
	brushSize = 10;
}

Editor::~Editor()
{

}

void Editor::mainLoop()
{
	char input = 0;
	bool inLoop = true;
	COLOURREF tempColour;
	Sprite tempSprite;
	Sprite pointerSprite = Sprite::pointer(pointer, brushSize, pointerColour);
	while (inLoop)
	{
		pastPointer = pointer;

		input = _getch();
		
		//Sprite::pointer(pastPointer, brushSize, backgroundColour).draw(hdc);
		pointerSprite.draw(hdc, backgroundColour);
		activeSprite.draw(hdc, backgroundColour);

		switch (input)
		{
		case '/':
			propertiesEditor();
			break;
		case 27: // Esc
			inLoop = false;
			break;
		default:
			switch (input)
			{
			case 'w':
			case 'W':
				pointer.y -= brushSize;
				pointerSprite.movePixels(0, -1 * brushSize);
				break;
			case 's':
			case 'S':
				pointer.y += brushSize;
				pointerSprite.movePixels(0, 1 * brushSize);
				break;
			case 'a':
			case 'A':
				pointer.x -= brushSize;
				pointerSprite.movePixels(-1 * brushSize, 0);
				break;
			case 'd':
			case 'D':
				pointer.x += brushSize;
				pointerSprite.movePixels(1 * brushSize, 0);
				break;
			case '4':
				//clr 1

				// different pointer sprite shapes can be used
				//  with function pointers!!!!
				//  sprite[5] -> triangle -> etc;
				//  or have a function that creates a shape by types

				tempSprite = Sprite::rectangle(pointer, brushSize, primaryColour);
				activeSprite += tempSprite;
				break;
			case '5':
				//erase / remove / delete

				tempSprite = Sprite::rectangle(pointer, brushSize, primaryColour);
				activeSprite -= tempSprite;
				break;
			case '6':
				//clr 2

				tempSprite = Sprite::rectangle(pointer, brushSize, secondaryColour);
				activeSprite += tempSprite;
				break;
			case '8':
				//pick clr

				break;
			case -32: // α
				input = _getch();
				switch (input)
				{
				case 'H': // Up Arrow
					activeSprite.movePixels(0, -1 * brushSize);
					break;
				case 'P': // Down Arrow
					activeSprite.movePixels(0, 1 * brushSize);
					break;
				case 'K': // Left Arrow
					activeSprite.movePixels(-1 * brushSize, 0);
					break;
				case 'M': // Right Arrow
					activeSprite.movePixels(1 * brushSize, 0);
					break;
				default:
					break;
				}
			default:
				break;
			}
			break;
		}

		activeSprite.draw(hdc);

		if ((tempColour = GetPixelPosStruct(hdc, pointer)) != backgroundColour)
		{
			pointerSprite.draw(hdc, RGB(
				255 - GetRValue(tempColour), 
				255 - GetGValue(tempColour), 
				255 - GetBValue(tempColour)
			));
		}
		else
			pointerSprite.draw(hdc);
	}

}

void Editor::propertiesEditor()
{
	activeSprite.draw(hdc, backgroundColour);


	for (unsigned i = 0; i < brushSize; i++)
		for (unsigned j = 0; j < brushSize; j++)
			SetPixel(hdc, j + pastPointer.x, i + pastPointer.y, backgroundColour);

	std::cout << "Properties Menu\n";

	char input = 0;
	bool inLoop = true;
	std::string command = "";

	std::cout << "r-g-b + 0-255 + -> change rgb value of main colour\n";
	std::cout << "palette\n";
	std::cout << "bs -> display current brush size\n";
	std::cout << "bs + 1-150 -> change the brush size (bs10 - by default)\n";
	while (inLoop)
	{
		_putch('/');
		break;
	}

	system("CLS");
}

void Editor::updateSprite(char mode)
{

}

