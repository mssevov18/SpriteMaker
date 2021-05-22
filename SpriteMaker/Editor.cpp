#include "Editor.h"

Position getResolution()
{
	RECT desktop;
	const HWND hDesktop = GetConsoleWindow();
	GetWindowRect(hDesktop, &desktop);
	return Position(desktop.right, desktop.bottom);
}

char getDigitCount(int number)
{
	char count = 0;
	while (number != 0)
	{
		number = number / 10;
		count++;
	}
	return count;
}


Editor::Editor()
{
	HWND hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);
	resolution = getResolution();
	palette.push_back(GetPixel(hdc, 0, 0));
	palette.push_back(RGB(230, 230, 230));
	palette.push_back(RGB(190, 160, 75));
	palette.push_back(RGB(50, 100, 220));
	palette.push_back(RGB(150, 100, 220));
	palette.push_back(RGB(250, 100, 220));
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
	char temp = getDigitCount(brushSize);
	unsigned char R, G, B;
	bool inLoop = true;
	COLOURREF tempColour;
	Sprite tempSprite;
	Sprite pointerSprite = Sprite::pointer(pointer, brushSize, pointerColour);
	std::cout << "\t\tBrush Size:   ";
	while (inLoop)
	{
		showPalette(Position(-10, 0));

		activeSprite.draw(hdc);

		tempColour = GetPixelPosStruct(hdc, pointer);
		R = GetRValue(tempColour);
		G = GetGValue(tempColour);
		B = GetBValue(tempColour);
		pointerSprite.draw(hdc, RGB(
			(255 >= R ? 255 - R : R - 255),
			(255 >= G ? 255 - G : G - 255),
			(255 >= B ? 255 - B : B - 255),
			));
		std::cout << brushSize;

		pastPointer = pointer;

		input = _getch();
		
		pointerSprite.draw(hdc, backgroundColour);
		activeSprite.draw(hdc, backgroundColour);
		
		for (char i = 0; i < temp; i++)
			_putch(8);		
		for (char i = 0; i < temp; i++)
			_putch(' ');
		for (char i = 0; i < temp; i++)
			_putch(8);

		switch (input)
		{
		case '/':
			propertiesEditor();
			system("CLS");
			std::cout << "\t\tBrush Size:   ";
			pointerSprite.draw(hdc, backgroundColour);
			activeSprite.draw(hdc, backgroundColour);
			break;
		case 27: // Esc
			inLoop = false;
			break;
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
		case '7':
			//pick clr

			break;
		case '+':
			//increase brush size

			brushSize++;
			pointerSprite = Sprite::pointer(pointer, brushSize, pointerColour);
			temp = getDigitCount(brushSize);
			break;
		case '-':
			//decrease brush size

			brushSize--;
			pointerSprite = Sprite::pointer(pointer, brushSize, pointerColour);
			temp = getDigitCount(brushSize);
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
	}

}

void Editor::propertiesEditor()
{

	char input = 0;
	bool inLoop = true;
	std::string path = "";
	COLOURREF temp;
	while (inLoop)
	{
		system("CLS");

		std::cout << std::endl;
		std::cout << "Properties Menu\n";
		std::cout << "0. Palette\n";
		std::cout << "1. Brush Size\n";
		std::cout << "Ctrl + s. Save Sprite\n";
		std::cout << "Ctrl + l. Load Sprite\n";
		std::cout << "Esc. Go back\n";

		input = _getch();
		switch (input)
		{
		case 27:
			return;
		case '0':
			std::cout << std::endl;
			std::cout << "0. Create New Colour\n";
			std::cout << "1. Change Active Colours\n";
			std::cout << "Esc. Go back\n";
			
			input = _getch();
			switch (input)
			{
			case '0':
				temp = colourEditor();
				if (temp != NULL)
					palette.push_back(temp);
				break;
			case '1':

				break;
			default:
				break;
			}
			break;
		case '1':
			std::cout << "New Brush Size (old size: " << brushSize << "): ";
			std::cin >> brushSize;
			break;
		case 19: // Ctrl + s
			std::cout << std::endl;
			std::cout << "Filepath: ";
			std::cin >> path;
			saveSprite(path);
			break;
		case 12: // Ctrl + l
			std::cout << std::endl;
			std::cout << "Filepath: ";
			std::cin >> path;
			loadSprite(path);
			break;
		default:
			break;
		}


	}

	system("CLS");
}

void Editor::showPalette(Position rootPos)
{
	COLOURREF borderColour;
	unsigned char R, G, B;
	Sprite paletteBox = Sprite::rectangle(rootPos, 10, RGB(100, 100, 100));
	Sprite paletteBorder = Sprite::border(rootPos, 10, backgroundColour);
	for (size_t i = 0; i < palette.size(); i++)
	{
		paletteBox.draw(hdc, palette[i]);

		borderColour = GetPixelPosStruct(hdc, rootPos);
		rootPos.x += 10;
		R = GetRValue(borderColour);
		G = GetGValue(borderColour);
		B = GetBValue(borderColour);
		paletteBorder.draw(hdc, RGB(
			(255 >= R ? 255 - R : R - 255),
			(255 >= G ? 255 - G : G - 255),
			(255 >= B ? 255 - B : B - 255),
			));
		paletteBox.movePixels(10, 0);
		paletteBorder.movePixels(10, 0);

		if (i == 3)
		{
			paletteBox.movePixels(-30, 10);
			paletteBorder.movePixels(-30, 10);
			rootPos.y += 10;
		}
	}
}

COLOURREF Editor::colourEditor()
{
	char input, len;
	unsigned char R = 255, G = 255, B = 255;
	bool inLoop = true;
	COLOURREF out = RGB(R, G, B);
	Sprite box = Sprite::rectangle(Position(250, 50), 50, out);

	system("CLS");
	std::cout << "\n\n";
	std::cout << "| ## | R | G | B |\n";
	std::cout << "|----|---|---|---|\n";
	std::cout << "| +1 | 7 | 8 | 9 |\n";
	std::cout << "| =0 | 4 | 5 | 6 |\n";
	std::cout << "| -1 | 1 | 2 | 3 |\n\n\n";

	while (inLoop)
	{
		std::cout << "RGB (" << int(R) << ", " << int(G) << ", " << int(B) << ")";
		len = getDigitCount(R) + getDigitCount(G) + getDigitCount(B) + 13;

		box.draw(hdc, out);

		input = _getch();

		for (char i = 0; i < len; i++)
			_putch(8);
		for (char i = 0; i < len; i++)
			_putch(' ');
		for (char i = 0; i < len; i++)
			_putch(8);

		switch (input)
		{
		case 27:
			std::cout << "Enter. Save the new colour; Esc. Discard the new colour";
			input = _getch();
			switch (input)
			{
			case 27:
				return NULL;
			case 13:
				return out;
			default:
				break;
			}
			for (char i = 0; i < 56; i++)
				_putch(8);
			for (char i = 0; i < 56; i++)
				_putch(' ');
			for (char i = 0; i < 56; i++)
				_putch(8);
			break;
		case 13:
			return out;
		case '7':
			R++;
			break;
		case '4':
			R = 0;
			break;
		case '1':
			R--;
			break;
		case '8':
			G++;
			break;
		case '5':
			G = 0;
			break;
		case '2':
			G--;
			break;
		case '9':
			B++;
			break;
		case '6':
			B = 0;
			break;
		case '3':
			B--;
			break;
		default:
			break;
		}

		if (R > 255)
			R = 0;
		if (R < 0)
			R = 255;

		if (G > 255)
			G = 0;
		if (G < 0)
			G = 255;

		if (B > 255)
			B = 0;
		if (B < 0)
			B = 255;

		out = RGB(R, G, B);
	}
	
	return COLOURREF();
}

void Editor::updateSprite(char mode)
{

}

void Editor::saveSprite(std::string path)
{
}

void Editor::loadSprite(std::string path)
{
}

