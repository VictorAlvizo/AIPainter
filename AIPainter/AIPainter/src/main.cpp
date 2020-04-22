#include <iostream>
#include <vector>
#include <Windows.h>
#include "PPMLibary.h"

struct Color {
	Color() {}

	Color(int r, int g, int b)
		:m_R(r), m_G(g), m_B(b) {}

	void SetColor(Pixel pixel) {
		m_R = pixel.r;
		m_G = pixel.g;
		m_B = pixel.b;
	}

	int m_R, m_G, m_B;

	bool operator==(const Color& color) {
		return (this->m_R == color.m_R && this->m_G == color.m_G && this->m_B == color.m_B) ? true : false;
	}
};

void Draw(const PPMLibary& imageData);
void ChangeColor(Color newColor);
void KeyInput(int value);

int main() {
	LPCWSTR windowTitle = L"Untitled - Paint"; //Get handle to paint program
	HWND window = FindWindow(NULL, windowTitle);

	while (window == NULL) { //Wait until program has a handle on paint
		std::cout << "Start up paint" << std::endl;
		window = FindWindow(NULL, windowTitle);
		Sleep(1000);
	}

	std::cout << "Handle ready, loading image" << std::endl;
	PPMLibary image("src/Images/HappyBox.ppm");

	if (image.isLoaded()) {
		std::cout << "Starting drawing" << std::endl;
		Draw(image);
		std::cout << "Image has finished drawing" << std::endl;
	}
	else {
		std::cout << "Image failed to load" << std::endl;
	}

	std::cin.get();
	return 0;
}

void Draw(const PPMLibary& image) {
	INPUT click[2] = { 0 }; //Clicking action, important for buttons in paint
	click[0].type = INPUT_MOUSE;
	click[1].type = INPUT_MOUSE;
	click[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	click[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	int currentX = 8, currentY = 148; //Position of the top-left of the paint canvas
	Color currentColor(0, 0, 0);
	Color newColor;

	bool exit = false; //Needed to exit out the nested for-loop

	SetCursorPos(635, 79); //Change brush size
	SendInput(2, click, sizeof(INPUT));
	Sleep(500); //Adjust sleep time according to computer, if to fast may glitch
	SetCursorPos(595, 224);
	SendInput(2, click, sizeof(INPUT));
	Sleep(500);

	for (int i = 0; i < image.getHeight(); i++) {
		for (int j = 0; j < image.getWidth(); j++) {
			newColor.SetColor(image.getPixel(j, i)); //Get pixel at that position

			if (!(currentColor == newColor)) { //If it's not the same color, change it
				currentColor = newColor;
				Sleep(100);
				ChangeColor(currentColor);
				Sleep(100);
			}

			if (GetAsyncKeyState(VK_ESCAPE)) { //Press 'escape' key, force exit
				exit = true;
				break;
			}

			currentX += 4;
			SetCursorPos(currentX, currentY);
			SendInput(2, click, sizeof(INPUT));

			if (currentX >= 1420) { //Once this x is reached, paint canvas horizontally to far out
				break;
			}
		}

		//Reset Positions but set cursor down
		currentX = 8; 
		currentY += 4;

		if (exit || currentY >= 681) { //Has hit y limit or forcing an exit
			break;
		}
	}
}

void ChangeColor(Color newColor) {
	INPUT click[2] = { 0 };
	click[0].type = INPUT_MOUSE;
	click[1].type = INPUT_MOUSE;
	click[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	click[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SetCursorPos(989, 75); //Click on edit colors
	SendInput(2, click, sizeof(INPUT));
	Sleep(500); //Wait for dialog box to open

	SetCursorPos(881, 436); //Go to red box
	SendInput(2, click, sizeof(INPUT));
	KeyInput(newColor.m_R);

	SetCursorPos(880, 459); //Go to green box
	SendInput(2, click, sizeof(INPUT));
	KeyInput(newColor.m_G);

	SetCursorPos(888, 482); //Go to blue box
	SendInput(2, click, sizeof(INPUT));
	KeyInput(newColor.m_B);

	SetCursorPos(801, 509); //Add color
	SendInput(2, click, sizeof(INPUT));

	SetCursorPos(487, 503); //Click ok
	SendInput(2, click, sizeof(INPUT));
	Sleep(300);
}

void KeyInput(int value) {
	std::vector<int> digits;

	while (value / 10 != 0) { //Get each single digit in a number. Example: 255 = {5, 5, 2}
		digits.push_back(value % 10);
		value /= 10;
	}
	digits.push_back(value);
	std::reverse(digits.begin(), digits.end()); //Reverse it so input can be correct; 255 = {2, 5, 5}

	INPUT input[6] = { 0 };

	for (int i = 0; i < 3; i++) { //First 3 inputs will be backspaces
		input[i].type = INPUT_KEYBOARD;
		input[i].ki.wVk = VK_BACK;
	}

	for (int i = 3; i < digits.size() + 3; i++) {
		input[i].type = INPUT_KEYBOARD;
		input[i].ki.wVk = VkKeyScanA('0' + digits[i - 3]); //Don't convert to ASCII char, get the vkcode value of 2
	}
	
	SendInput(digits.size() + 3, input, sizeof(INPUT)); //Only input what I have
}