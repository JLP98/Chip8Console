#include "chip8.h"
#include <iostream>
#include <fstream>

Chip8* Chip8::instance = 0;

Chip8* Chip8::getInstance()
{
	if (instance == 0)
	{
		instance = new Chip8();
	}
	return instance;
}

Chip8::Chip8()
{
	romLoaded = false;
	std::fill(memory, memory + 4096, 0);
	std::fill(registers, registers + 16, 0);
	std::fill(stack, stack + 16, 0);
	indexRegister = 0;
	stackPointer = 0;
	delayTimer = 0;
	soundTimer = 0;
}

void Chip8::loadRom(string romPath) {
	std::ifstream romFile;
	romFile.open(romPath, std::ios::binary);
	romFile.seekg(0, romFile.end);
	fileSize = romFile.tellg();
	romFile.seekg(0, romFile.beg);
	for (short int i = MEMORY_START_POS; i <= MEMORY_START_POS + fileSize; i++) {
		romFile.read(reinterpret_cast<char*>(&memory[i]), 1);
	}
	romFile.close();
	romLoaded = true;
}

void Chip8::romCycle() {
	for (short int programCounter = MEMORY_START_POS; programCounter <= MEMORY_START_POS + fileSize; programCounter += 2) {
		uint16_t opcode = (memory[programCounter] << 8u) | memory[programCounter + 1];
		executeInstruction(opcode);
		if (delayTimer > 0) {
			delayTimer--;
		}
		if (soundTimer > 0) {
			soundTimer--;
		}
	}
}

void Chip8::executeInstruction(uint16_t instruction) {
	uint16_t leftNibble = instruction & 0xF000;
	uint16_t right3Nibbles = instruction & 0x0FFF;
	switch (leftNibble) {
	case 0x0000:
	{
		if (right3Nibbles == 0x00E0) {
			clearScreen();
		}
		else if (right3Nibbles == 0x00EE) {
			returnFromRoutine();
		}
	}
	case 0x1000:
	{
		jumpAddress(right3Nibbles);
		break;
	}
	case 0x2000:
	{
		callRoutine(right3Nibbles);
		break;
	}
	case 0x3000:
	{
		skipNextEqual(right3Nibbles);
		break;
	}
	case 0x4000:
	{
		skipNextNotEqual(right3Nibbles);
		break;
	}
	case 0x5000:
	{
		skipNextEqualRegs(right3Nibbles);
		break;
	}
	case 0x6000:
	{
		setRegister(right3Nibbles);
		break;
	}
	case 0x7000:
		addToRegister(right3Nibbles);
		break;
	case 0x8000:
	{
		registerChanges(right3Nibbles);
		break;
	}
	case 0x9000:
	{
		skipNextNotEqualRegs(right3Nibbles);
		break;
	}
	case 0xA000:
	{
		setIndexRegister(right3Nibbles);
		break;
	}
	case 0xB000:
	{
		jumpAddressPlusV0(right3Nibbles);
		break;
	}
	case 0xC000:
	{
		setRegisterRand(right3Nibbles);
		break;
	}
	case 0xD000:
	{
		drawPixelsToDisplay(right3Nibbles);
		break;
	}
	case 0xE000:
	{
		keyOperations(right3Nibbles);
		break;
	}
	case 0xF000:
	{
		extraOperations(right3Nibbles);
		break;
	}
	}
}

void Chip8::clearScreen() { // 00E0

}

void Chip8::returnFromRoutine() { // 00EE
	stackPointer--;
	programCounter = stack[stackPointer];
}

void Chip8::jumpAddress(uint16_t  address) { // 1NNN
	programCounter = address;
}
void Chip8::callRoutine(uint16_t  address) { // 2NNN
	stack[stackPointer] = programCounter;
	stackPointer++;
	programCounter = address;
}
void Chip8::skipNextEqual(uint16_t  address) { // 3NNN
	uint8_t registerIndex = address >> 8u;
	uint8_t value = address & 0x00FF;
	if (registerIndex > 16) {
		return;
	}
	if (registers[registerIndex] == value) {
		programCounter += 2;
	}
}
void Chip8::skipNextNotEqual(uint16_t  address) { // 4NNN
	uint8_t registerIndex = address >> 8u;
	uint8_t value = address & 0x00FF;
	if (registerIndex > 16) {
		return;
	}
	if (registers[registerIndex] != value) {
		programCounter += 2;
	}
}
void Chip8::skipNextEqualRegs(uint16_t  address) { // 5NNN
	uint8_t registerIndexX = address >> 8u;
	uint8_t registerIndexY = address >> 4u;
	if (registerIndexX > 16 || registerIndexY > 16) {
		return;
	}
	if (registers[registerIndexX] == registers[registerIndexY]) {
		programCounter += 2;
	}
}
void Chip8::skipNextNotEqualRegs(uint16_t  address) { // 9XY0
	uint8_t registerIndexX = address >> 8u;
	uint8_t registerIndexY = address >> 4u;
	if (registerIndexX > 16 || registerIndexY > 16) {
		return;
	}
	if (registers[registerIndexX] != registers[registerIndexY]) {
		programCounter += 2;
	}
}
void Chip8::setRegister(uint16_t  address) { // 6XNN
	uint8_t registerIndex = address >> 8u;
	uint8_t value = address & 0x00FF;
	if (registerIndex > 16) {
		return;
	}
	registers[registerIndex] = value;
}
void Chip8::addToRegister(uint16_t  address) { // 7XNN
	uint8_t registerIndex = address >> 8u;
	uint8_t value = address & 0x00FF;
	if (registerIndex > 16) {
		return;
	}
	registers[registerIndex] += value;
}
void Chip8::setIndexRegister(uint16_t  address) { // ANNN
	indexRegister = address;
}
void Chip8::jumpAddressPlusV0(uint16_t  address) { // BNNN
	programCounter = address + registers[0];
}
void Chip8::setRegisterRand(uint16_t  address) { // CXNN
	uint8_t registerIndex = address >> 8u;
	uint8_t value = address & 0x00FF;
	if (registerIndex > 16) {
		return;
	}
	registers[registerIndex] = (rand() % 255) & value;
}
void Chip8::drawPixelsToDisplay(uint16_t  address) { // DXYN

}
void Chip8::registerChanges(uint16_t  instruction) { // 8XYN
	uint8_t operation = instruction & 0x000F;
	uint8_t registerIndexX = instruction >> 8u;
	uint8_t registerIndexY = instruction >> 4u;
	switch (operation)
	{
	case 0x0000:
	{
		registers[registerIndexX] = registers[registerIndexY];
		break;
	}
	case 0x0001:
	{
		registers[registerIndexX] = registers[registerIndexX] | registers[registerIndexY];
		break;
	}
	case 0x0002:
	{
		registers[registerIndexX] = registers[registerIndexX] & registers[registerIndexY];
		break;
	}
	case 0x0003:
	{
		registers[registerIndexX] = registers[registerIndexX] ^ registers[registerIndexY];
		break;
	}
	case 0x0004:
	{
		uint16_t sum = registers[registerIndexX] + registers[registerIndexY];
		if (sum > 255) {
			registers[15] = 1;
		}
		else {
			registers[15] = 0;
		}
		registers[registerIndexX] = sum;
		break;
	}
	case 0x0005:
	{
		if (registers[registerIndexX] > registers[registerIndexY]) {
			registers[15] = 1;
		}
		else {
			registers[15] = 0;
		}
		registers[registerIndexX] -= registers[registerIndexY];
		break;
	}
	case 0x0006:
	{
		registers[15] = registerIndexX % 2;
		registers[registerIndexX] >>= 1;
		break;
	}
	case 0x0007:
	{
		if (registers[registerIndexX] < registers[registerIndexY]) {
			registers[15] = 1;
		}
		else {
			registers[15] = 0;
		}
		registers[registerIndexX] = registers[registerIndexY] - registers[registerIndexX];
		break;
	}
	case 0x000E:
	{
		if (registerIndexX >= 8) {
			registers[15] = 1;
		}
		else {
			registers[15] = 0;
		}
		registers[registerIndexX] <<= 1;
		break;
	}
	}
}
void Chip8::keyOperations(uint16_t  instruction) { // EX9E & EXA1
	uint8_t registerKey = instruction >> 8u;
	// if(key()==Vx) - Skip next instruction
	// if(key()!=Vx) - Skip the next instruction
}
void Chip8::extraOperations(uint16_t  instruction) { // FXNN
	uint8_t registerIndex = instruction >> 8u;
	uint8_t operation = instruction & 0x00FF;
	switch (operation) {
	case 0x0007:
	{
		registers[registerIndex] = delayTimer;
		break;
	}
	case 0x000A:
	{
		// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event) 
		break;
	}
	case 0x0015:
	{
		delayTimer = registers[registerIndex];
		break;
	}
	case 0x0018:
	{
		soundTimer = registers[registerIndex];
		break;
	}
	case 0x001E:
	{
		indexRegister += registers[registerIndex];
		break;
	}
	case 0x0029:
	{
		// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font. 
		break;
	}
	case 0x0033:
	{
		// BCD
		break;
	}
	case 0x0055:
	{
		for (int i = 0; i <= registerIndex; i++) {
			memory[indexRegister + i] = registers[registerIndex];
		}
		break;
	}
	case 0x0065:
	{
		for (int i = 0; i <= registerIndex; i++) {
			registers[registerIndex] = memory[indexRegister + i];
		}
		break;
	}
	}
}