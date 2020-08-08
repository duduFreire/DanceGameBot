#include <iostream>
#include "windows.h"
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

void pressKey(char key)
{
	INPUT input;
	WORD vkey = key;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void clickMouse(int x, int y)
{
	INPUT Inputs[3] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dx = (long)x * 65535.0 / 1920.0; // desired 65535,65535X coordinate
	Inputs[0].mi.dy = (long)y * 65535.0 / 1080.0; // desired Y coordinate
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	Inputs[1].type = INPUT_MOUSE;
	Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	Inputs[2].type = INPUT_MOUSE;
	Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(3, Inputs, sizeof(INPUT));
}

int orangeArrowPos[2];
int orangeArrowCol[] = { 213, 108, 24 };

int yellowArrowPos[2];
int yellowArrowCol[] = { 255, 226, 5 };

int greenArrowPos[2];
int greenArrowCol[] = { 55, 174, 90 };

int blueArrowPos[2];
int blueArrowCol[] = { 0, 61, 208 };

bool pressArrow(HDC& hdc, int pos[], int col[], char key)
{
	DWORD color = GetPixel(hdc, pos[0], pos[1]);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if (r - col[0] == 0 && g - col[1] == 0 && b - col[2] == 0) {
		pressKey(key);

		return true;
	}

	return false;
}

// 1030 703

void readCFG()
{
	std::ifstream cfg;
	cfg.open("cfg.txt");

	std::string line;

	for (int i = 0; std::getline(cfg, line); i++) {
		std::istringstream iss(line);
		std::vector<std::string> result{
			std::istream_iterator<std::string>(iss), {}
		};

		switch (i) {
		case 0:
			orangeArrowPos[0] = std::stoi(result[0]);
			orangeArrowPos[1] = std::stoi(result[1]);
			break;
		case 1:
			yellowArrowPos[0] = std::stoi(result[0]);
			yellowArrowPos[1] = std::stoi(result[1]);
			break;
		case 2:
			greenArrowPos[0] = std::stoi(result[0]);
			greenArrowPos[1] = std::stoi(result[1]);
			break;
		case 3:
			blueArrowPos[0] = std::stoi(result[0]);
			blueArrowPos[1] = std::stoi(result[1]);
			break;
		}
	}

	cfg.close();
}

int main()
{
	HDC hdc = GetDC(NULL);

	while (!GetAsyncKeyState(VK_F2)) {};

	readCFG();

	std::cout << "Bot started\n";

	bool left = false, up = false, down = false, right = false;
	float timeElapsed = 0.0f;
	while (!GetAsyncKeyState(VK_F10)) {
		if (timeElapsed / 1000.0f > 2.0f) {
			timeElapsed = 0;
			DWORD color = GetPixel(hdc, 1066, 821);
			//DWORD color = GetPixel(hdc, 1435, 880); // gus
			int r = GetRValue(color) - 237;
			int g = GetGValue(color) - 128;
			int b = GetBValue(color) - 121;

			if (r == 0 && g == 0 && b == 0) {
				clickMouse(959, 949);
				//clickMouse(1095, 1077); // gus
				Sleep(500);
				clickMouse(959, 949);
				Sleep(500);
				clickMouse(959, 949);
				Sleep(500);
				clickMouse(959, 949);
			}
		}

		if (!left)
			left = pressArrow(hdc, orangeArrowPos, orangeArrowCol, VK_LEFT);
		else left = false;
		if (!up)
			up = pressArrow(hdc, yellowArrowPos, yellowArrowCol, VK_UP);
		else up = false;
		if (!down)
			down = pressArrow(hdc, greenArrowPos, greenArrowCol, VK_DOWN);
		else down = false;
		if (!right)
			right = pressArrow(hdc, blueArrowPos, blueArrowCol, VK_RIGHT);
		else right = false;

		timeElapsed += 10.0f;

		Sleep(10);
	}
}