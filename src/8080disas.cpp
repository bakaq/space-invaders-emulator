#include <vector>
#include <stdio.h>
#include <string>

#include "8080disas.hpp"


int disas8080op(std::vector<unsigned char> code_buffer, int pc){
	int opbytes = 1;
	std::string opcode = "";
	
	switch(code_buffer[pc]){
		case 0x00: opcode = "NOP"; break;
		case 0xC3: 
			char fs[13];
			sprintf(fs, "JMP %02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		default: opcode = "Not Implemented"; break;
	}


	printf("%04x: ", pc);
	for(int i = 0; i < 3; ++i){
		if(i < opbytes){
			printf("%02X ", code_buffer[pc+i]);
		}else{
			printf("   ");
		}
	}
	printf("- %s\n", opcode.c_str());
	return opbytes;
}
