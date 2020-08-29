#include "ConsoleRenderer.h"

#include <exception>
#include <string>
#include <Windows.h>

ConsoleRenderer::ConsoleRenderer() {
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(this->hConsole, ENABLE_WINDOW_INPUT);
	this->hBuffer1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	this->hBuffer2 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	SetConsoleActiveScreenBuffer(this->hBuffer1);
}


ConsoleRenderer::~ConsoleRenderer() {
}

void ConsoleRenderer::Render(const uint8_t* buffer) {
	CHAR_INFO char_info[64 * 32];
	for (int i = 0; i < 32 * 64; i++) {
		char_info[i].Char.AsciiChar = L'A';
		char_info[i].Attributes = buffer[i] >= 1 ? 0xFF : 0x00;
	}

	SMALL_RECT output_rect{ 0, 0, 64, 32 };
	if (0 == WriteConsoleOutput(this->hBuffer2, char_info, { 64, 32 }, { 0, 0 }, &output_rect)) {
		DWORD error = GetLastError();
		throw std::exception((std::string("Drawing failed") + _itoa(error, nullptr, 10)).c_str());
	}

	SetConsoleActiveScreenBuffer(this->hBuffer2);
	std::swap(this->hBuffer1, this->hBuffer2);

	SMALL_RECT screen_rect{ 0, 0, 64, 32 };
	SetConsoleWindowInfo(this->hBuffer1, true, &screen_rect);
	SetConsoleWindowInfo(this->hBuffer2, true, &screen_rect);
	SetConsoleWindowInfo(this->hConsole, true, &screen_rect);
	CONSOLE_CURSOR_INFO cursor_info{ 1, false };
	SetConsoleCursorInfo(this->hBuffer1, &cursor_info);
	SetConsoleCursorInfo(this->hBuffer2, &cursor_info);
	SetConsoleCursorInfo(this->hConsole, &cursor_info);
}