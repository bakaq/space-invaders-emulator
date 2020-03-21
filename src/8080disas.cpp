#include <vector>
#include <stdio.h>
#include <string>

#include "8080disas.hpp"



int disas8080opf(const std::vector<unsigned char> &code_buffer, int pc){
	int opbytes = 1;
	std::string opcode = disas8080op(code_buffer, pc, opbytes); 
	printf("%04X: ", pc);
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


std::string disas8080op(const std::vector<unsigned char> &code_buffer, int pc, int &opbytes){
	std::string opcode = "";
	char fs[30];
	switch(code_buffer[pc]){
		case 0x00: opcode = "NOP"; break;
		case 0x05: opcode = "DCR B"; break;
		case 0x06:
			sprintf(fs, "MVI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x0F: opcode = "RRC"; break;
		case 0x11:
			sprintf(fs, "LXI D, #$%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x13: opcode = "INX D"; break;
		case 0x1A: opcode = "LDAX D"; break;
		case 0x21: 
			sprintf(fs, "LXI H, $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x22:
			sprintf(fs, "SHLD $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x23: opcode = "INX H"; break;
		case 0x27: opcode = "DAA"; break;
		case 0x2B: opcode = "DCX H"; break;
		case 0x31:
			sprintf(fs, "LXI #$%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x32: 
			sprintf(fs, "STA $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x35: opcode = "DCR M"; break;
		case 0x3A: 
			sprintf(fs, "LDA $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x3C: opcode = "INR A"; break;
		case 0x3D: opcode = "DCR A"; break;
		case 0x3E:
			sprintf(fs, "MVI A, #$%02X",code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x40: opcode = "MOV B, B"; break;
		case 0x41: opcode = "MOV B, C"; break;
		case 0x42: opcode = "MOV B, D"; break;
		case 0x43: opcode = "MOV B, E"; break;
		case 0x44: opcode = "MOV B, H"; break;
		case 0x45: opcode = "MOV B, L"; break;
		case 0x46: opcode = "MOV B, M"; break;
		case 0x47: opcode = "MOV B, A"; break;
		case 0x48: opcode = "MOV C, B"; break;
		case 0x49: opcode = "MOV C, C"; break;
		case 0x4A: opcode = "MOV C, D"; break;
		case 0x4B: opcode = "MOV C, E"; break;
		case 0x4C: opcode = "MOV C, H"; break;
		case 0x4D: opcode = "MOV C, L"; break;
		case 0x4E: opcode = "MOV C, M"; break;
		case 0x4F: opcode = "MOV C, A"; break;
		case 0x50: opcode = "MOV D, B"; break;
		case 0x51: opcode = "MOV D, C"; break;
		case 0x52: opcode = "MOV D, D"; break;
		case 0x53: opcode = "MOV D, E"; break;
		case 0x54: opcode = "MOV D, H"; break;
		case 0x55: opcode = "MOV D, L"; break;
		case 0x56: opcode = "MOV D, M"; break;
		case 0x57: opcode = "MOV D, A"; break;
		case 0x58: opcode = "MOV E, B"; break;
		case 0x59: opcode = "MOV E, C"; break;
		case 0x5A: opcode = "MOV E, D"; break;
		case 0x5B: opcode = "MOV E, E"; break;
		case 0x5C: opcode = "MOV E, H"; break;
		case 0x5D: opcode = "MOV E, L"; break;
		case 0x5E: opcode = "MOV E, M"; break;
		case 0x5F: opcode = "MOV E, A"; break;
		case 0x60: opcode = "MOV H, B"; break;
		case 0x61: opcode = "MOV H, C"; break;
		case 0x62: opcode = "MOV H, D"; break;
		case 0x63: opcode = "MOV H, E"; break;
		case 0x64: opcode = "MOV H, H"; break;
		case 0x65: opcode = "MOV H, L"; break;
		case 0x66: opcode = "MOV H, M"; break;
		case 0x67: opcode = "MOV H, A"; break;
		case 0x68: opcode = "MOV L, B"; break;
		case 0x69: opcode = "MOV L, C"; break;
		case 0x6A: opcode = "MOV L, D"; break;
		case 0x6B: opcode = "MOV L, E"; break;
		case 0x6C: opcode = "MOV L, H"; break;
		case 0x6D: opcode = "MOV L, L"; break;
		case 0x6E: opcode = "MOV L, M"; break;
		case 0x6F: opcode = "MOV L, A"; break;
		case 0x70: opcode = "MOV M, B"; break;
		case 0x71: opcode = "MOV M, C"; break;
		case 0x72: opcode = "MOV M, D"; break;
		case 0x73: opcode = "MOV M, E"; break;
		case 0x74: opcode = "MOV M, H"; break;
		case 0x75: opcode = "MOV M, L"; break;
		case 0x76: opcode = "MOV HLT"; break;
		case 0x77: opcode = "MOV M, A"; break;
		case 0x78: opcode = "MOV A, B"; break;
		case 0x79: opcode = "MOV A, C"; break;
		case 0x7A: opcode = "MOV A, D"; break;
		case 0x7B: opcode = "MOV A, E"; break;
		case 0x7C: opcode = "MOV A, H"; break;
		case 0x7D: opcode = "MOV A, L"; break;
		case 0x7E: opcode = "MOV A, M"; break;
		case 0x7F: opcode = "MOV A, A"; break;
		case 0xA7: opcode = "ANA A"; break;
		case 0xAF: opcode = "XRA A"; break;
		case 0xC1: opcode = "POP B"; break;
		case 0xC2: 
			sprintf(fs, "JNZ $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xC3: 
			sprintf(fs, "JMP $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xC5: opcode = "PUSH B"; break;
		case 0xC6:
			sprintf(fs, "ADI $%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xC9: opcode = "RET"; break;
		case 0xCA:
			sprintf(fs, "JZ $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xCD: 
			sprintf(fs, "CALL $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xD1: opcode = "POP D"; break;
		case 0xD2:
			sprintf(fs, "JNC $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xD5: opcode = "PUSH D"; break;
		case 0xDA: 
			sprintf(fs, "JC $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xDB: 
			sprintf(fs, "IN #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xE1: opcode = "POP H"; break;
		case 0xE5: opcode = "PUSH H"; break;
		case 0xF1: opcode = "POP PSW"; break;
		case 0xF5: opcode = "PUSH PSW"; break;
		case 0xFB: opcode = "EI"; break;
		case 0xFE:
			sprintf(fs, "CPI $%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		default: opcode = "Not Implemented"; break;
	}
	return opcode;
}
