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
protected:
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
	uint8_t memory[4096];
	uint8_t registers[16];
	uint8_t stack[16];
	uint16_t indexRegister;
	uint16_t programCounter;
	uint8_t stackPointer;
	uint8_t delayTimer;
	uint8_t soundTimer;
	bool romLoaded;
	std::streampos fileSize;
	const short int MEMORY_START_POS = 512;
};

#endif