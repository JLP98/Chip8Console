// Chip8Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include "chip8.h"
#include "ConsoleRenderer.h"
#include <thread>
#include <chrono>

int main()
{
	int refreshRate = 960;
	int delay = 1000000 / refreshRate;
	std::string filePath = "TETRIS";
	Chip8* chip8 = Chip8::getInstance();
	(*chip8).loadRom(filePath);
	ConsoleRenderer renderer;
	while (true) {
		(*chip8).romCycle();
		if ((*chip8).getDrawGraphics()) {
			uint8_t* graphics = (*chip8).getDisplay();
			renderer.Render(graphics);
		}
		(*chip8).setKey(0x1, GetAsyncKeyState('1') & 0x8000);
		(*chip8).setKey(0x2, GetAsyncKeyState('2') & 0x8000);
		(*chip8).setKey(0x3, GetAsyncKeyState('3') & 0x8000);
		(*chip8).setKey(0xC, GetAsyncKeyState('4') & 0x8000);

		(*chip8).setKey(0x4, GetAsyncKeyState('Q') & 0x8000);
		(*chip8).setKey(0x5, GetAsyncKeyState('W') & 0x8000);
		(*chip8).setKey(0x6, GetAsyncKeyState('E') & 0x8000);
		(*chip8).setKey(0xD, GetAsyncKeyState('R') & 0x8000);

		(*chip8).setKey(0x7, GetAsyncKeyState('A') & 0x8000);
		(*chip8).setKey(0x8, GetAsyncKeyState('S') & 0x8000);
		(*chip8).setKey(0x9, GetAsyncKeyState('D') & 0x8000);
		(*chip8).setKey(0xE, GetAsyncKeyState('F') & 0x8000);

		(*chip8).setKey(0xA, GetAsyncKeyState('Z') & 0x8000);
		(*chip8).setKey(0x0, GetAsyncKeyState('X') & 0x8000);
		(*chip8).setKey(0xB, GetAsyncKeyState('C') & 0x8000);
		(*chip8).setKey(0xF, GetAsyncKeyState('V') & 0x8000);
		std::this_thread::sleep_for(std::chrono::microseconds(delay));
	}
}

