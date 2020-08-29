#pragma once
#ifndef CHIP8_H
#define CHIP8_H
#include <string>
using std::string;

class Chip8 {

public:
	static Chip8* getInstance();
	void romCycle();
	void loadRom(string romPath);
	bool getDrawGraphics();
	uint8_t* getDisplay();
	void setKey(int keyNumber, bool state);
private:
	static Chip8* instance;
	Chip8();
	void executeInstruction(uint16_t instruction);
	void clearScreen();
	void returnFromRoutine();
	void jumpAddress(uint16_t  address);
	void callRoutine(uint16_t  address);
	void skipNextEqual(uint16_t  address);
	void skipNextNotEqual(uint16_t  address);
	void skipNextEqualRegs(uint16_t  address);
	void skipNextNotEqualRegs(uint16_t  address);
	void setRegister(uint16_t  address);
	void addToRegister(uint16_t  address);
	void setIndexRegister(uint16_t  address);
	void jumpAddressPlusV0(uint16_t  address);
	void setRegisterRand(uint16_t  address);
	void drawPixelsToDisplay(uint16_t  address);
	void registerChanges(uint16_t  instruction);
	void keyOperations(uint16_t  instruction);
	void extraOperations(uint16_t  instruction);
	static const uint16_t totalPixels = 64 * 32;
	static const short int MEMORY_START_POS = 512;
	uint8_t display[totalPixels];
	uint8_t key[16];
	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t stack[16];
	uint16_t indexRegister;
	uint16_t programCounter;
	uint8_t stackPointer;
	uint8_t delayTimer;
	uint8_t soundTimer;
	bool romLoaded;
	bool drawGraphics;
	bool keyPressed;
	std::streampos fileSize;

	uint8_t fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};

#endif