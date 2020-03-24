#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

#include "8080.hpp"
#include "8080disas.hpp"

// Debug mode
bool DEBUG = false;

// Shift resister
uint16_t shift_register = 0x0000;
uint8_t shift_offset = 0x00;

// Functions
void execute(const std::vector<uint8_t> &code);
void machine_IN(c8080 &chip, int port);
void machine_OUT(const c8080 &chip, int port);

int main(int argc, char* argv[]){
	
	// Flags
	bool disas_flag = false;
	
	// Rom
	std::string rom_name = "";

	// Parse command line arguments
	for(int i = 1; i < argc; ++i){
		if(std::string(argv[i]) == "-d"){
			disas_flag = true;
		}else if(std::string(argv[i]) == "-D"){
			DEBUG = true;
		}else{
			rom_name = std::string(argv[i]);
		}
	}

	if(DEBUG){
		if(rom_name != ""){
			printf("Rom name: %s\n", rom_name.c_str());
		}

		if(disas_flag){
			printf("Disassembly mode\n");
		}
	}

	// Read rom to buffer
	std::ifstream rf(rom_name.c_str(), std::ios::binary);
	if(!rf){
		printf("Couldn't open file!\n");
		return 1;
	}
	std::vector<uint8_t> code_buffer(std::istreambuf_iterator<char>(rf), {});

	// Execute or disassembly
	if(disas_flag){
		unsigned int pc = 0;
		while(pc < code_buffer.size()){
			pc += disas8080opf(code_buffer, pc);
		}
	}else{
		// Run
		execute(code_buffer);
	}

	return 0;
}


void execute(const std::vector<uint8_t> &code){
	c8080 chip(code);
	bool running = true;
	while(running){
		uint8_t op = chip.memory[chip.pc];
		if(op == 0xDB){ // IN
			uint8_t port = chip.memory[chip.pc + 1];
			machine_IN(chip, port);
			chip.pc += 2;
		}else if(op == 0xD3){ // OUT
			uint8_t port = chip.memory[chip.pc + 1];
			machine_OUT(chip, port);
			chip.pc += 2;
		}else{
			if(DEBUG){
				// Debugging
				int itt = 0;
				printf("%sIterations: ", chip.get_state().c_str());
				scanf("%d", &itt);
				for(int i = 0; i < itt; ++i){
					chip.cycle();
				}
			}else{
				chip.cycle();
			}
		}
	}
}

void machine_IN(c8080 &chip, int port){
	switch(port){
		case 3:
			chip.a = (shift_register >> (8-shift_offset)) & 0xFF;
			break;
	}
}

void machine_OUT(const c8080 &chip, int port){
	switch(port){
		case 2:
			shift_offset = chip.a & 0x7;
			break;
		case 4:
			shift_register = ((chip.a << 8) & 0xFF00) | ((shift_register >> 8) & 0xFF);
			break;
	}
}

