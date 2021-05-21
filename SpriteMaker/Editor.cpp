#include "Editor.h"

Editor::Editor()
{
	hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);
	backgroundColour = GetPixel(hdc, 0, 0);
	mainColour = RGB(190, 160, 75);
	secondaryColour = RGB(50, 100, 220);
	pointer.x = 50;
	pointer.y = 50;
	pixelScale = 10;
}

Editor::~Editor()
{

}

void Editor::mainLoop()
{
	char input = 0;
	bool inLoop = true;
	Sprite pointerSprite;
	while (inLoop)
	{
		pastPointer = pointer;

		input = _getch();
		
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
				pointer.y -= pixelScale;
				break;
			case 's':
			case 'S':
				pointer.y += pixelScale;
				break;
			case 'a':
			case 'A':
				pointer.x -= pixelScale;
				break;
			case 'd':
			case 'D':
				pointer.x += pixelScale;
				break;
			case '4':
				//clr 1

				//Old code
				//for (unsigned i = 0; i < pixelScale; i++)
				//	for (unsigned j = 0; j < pixelScale; j++)
				//		activeSprite.addPixel
				//		(Pixel(
				//			Position(
				//				int(j + pointer.x),
				//				int(i + pointer.y)),
				//			mainColour)
				//		);

				pointerSprite = Sprite::rectangle(pointer, pixelScale, mainColour);
				activeSprite += pointerSprite;
				break;
			case '5':
				//erase / remove / delete
				
				//Old code
				//for (unsigned i = 0; i < pixelScale; i++)
				//	for (unsigned j = 0; j < pixelScale; j++)
				//		activeSprite.removePixel
				//		(Pixel(
				//			Position(
				//				int(j + pointer.x),
				//				int(i + pointer.y)),
				//			mainColour),
				//			0
				//		);

				pointerSprite = Sprite::rectangle(pointer, pixelScale, mainColour);
				activeSprite -= pointerSprite;
				break;
			case '6':
				//clr 2

				pointerSprite = Sprite::rectangle(pointer, pixelScale, secondaryColour);
				activeSprite += pointerSprite;
				break;
			case '8':
				//pick clr

				break;
			default:
				break;
			}
			spriteEditor();
			break;
		}
	}

}

void Editor::spriteEditor()
{
	activeSprite.draw(hdc, backgroundColour);
	
	for (unsigned i = 0; i < pixelScale; i++)
		for (unsigned j = 0; j < pixelScale; j++)
			SetPixel(hdc, j + pastPointer.x, i + pastPointer.y, backgroundColour);


	activeSprite.draw(hdc);

	for (unsigned i = 0; i < pixelScale; i++)
		for (unsigned j = 0; j < pixelScale; j++)
			SetPixel(hdc, j + pointer.x, i + pointer.y, mainColour);
}

void Editor::propertiesEditor()
{
	activeSprite.draw(hdc, backgroundColour);


	for (unsigned i = 0; i < pixelScale; i++)
		for (unsigned j = 0; j < pixelScale; j++)
			SetPixel(hdc, j + pastPointer.x, i + pastPointer.y, backgroundColour);

	std::cout << "Properties Menu\n";

	char input = 0;
	bool inLoop = true;
	std::string command = "";

	std::cout << "r-g-b + 0-255 -> change rgb value of main colour\n";
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
