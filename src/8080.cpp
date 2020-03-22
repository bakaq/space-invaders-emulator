#include <vector>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>

#include "8080.hpp"
#include "8080disas.hpp"

c8080::c8080(const std::vector<uint8_t>& code){
	// Initialize state
	this->a = 0;
	this->b = 0;
	this->c = 0;
	this->d = 0;
	this->e = 0;
	this->h = 0;
	this->l = 0;
	this->sp = 0;
	this->pc = 0;
	this->cc.z = 0;
	this->cc.s = 0;
	this->cc.p = 0;
	this->cc.cy = 0;
	this->cc.ac = 0;
	this->int_enable = 0;
	
	this->memory = std::vector<uint8_t>(0x10000, 0);
	for(unsigned int i = 0; i < code.size(); ++i){
		this->memory[i] = code[i];
	}
}


void c8080::cycle(){
	std::vector<uint8_t>& mem = this->memory;
	uint16_t& pc = this->pc;

	uint8_t op = mem[pc];
	disas8080opf(mem, pc);
	switch(op){
		case 0x00: break; // NOP
		case 0xC3: // JMP adr
			pc = ((mem[pc+2] << 8) | mem[pc+1]) - 1; 
			break;
		default:
			printf("Instruction not implemented!\n");
			exit(1);
	}
	pc += 1;
}
