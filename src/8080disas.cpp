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
		case 0x01: 
			sprintf(fs, "LXI B, #$%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x02: opcode = "STAX B"; break;
		case 0x03: opcode = "INX B"; break;
		case 0x04: opcode = "INR B"; break;
		case 0x05: opcode = "DCR B"; break;
		case 0x06:
			sprintf(fs, "MVI B, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x07: opcode = "RLC"; break;
		case 0x09: opcode = "DAD B"; break;
		case 0x0A: opcode = "LDAX B"; break;
		case 0x0B: opcode = "DCX B"; break;
		case 0x0C: opcode = "INR C"; break;
		case 0x0D: opcode = "DCR C"; break;
		case 0x0E: 
			sprintf(fs, "MVI C, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x0F: opcode = "RRC"; break;
		case 0x11:
			sprintf(fs, "LXI D, #$%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x12: opcode = "STAX D"; break;
		case 0x13: opcode = "INX D"; break;
		case 0x14: opcode = "INR D"; break;
		case 0x15: opcode = "DCR D"; break;
		case 0x16:
			sprintf(fs, "MVI D, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x17: opcode = "RAL"; break;
		case 0x19: opcode = "DAD D"; break;
		case 0x1A: opcode = "LDAX D"; break;
		case 0x1B: opcode = "DCX D"; break;
		case 0x1C: opcode = "INR E"; break;
		case 0x1D: opcode = "DCR E"; break;
		case 0x1E: 
			sprintf(fs, "MVI E, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x1F: opcode = "RAR"; break;
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
		case 0x24: opcode = "INR H"; break;
		case 0x25: opcode = "DCR H"; break;
		case 0x26: 
			sprintf(fs, "MVI H, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x27: opcode = "DAA"; break;
		case 0x29: opcode = "DAD H"; break;
		case 0x2A: 
			sprintf(fs, "LHLD $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x2B: opcode = "DCX H"; break;
		case 0x2C: opcode = "INR L"; break;
		case 0x2D: opcode = "DCR L"; break;
		case 0x2E: 
			sprintf(fs, "MVI L, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x2F: opcode = "CMA"; break;
		case 0x31:
			sprintf(fs, "LXI SP, #$%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x32: 
			sprintf(fs, "STA $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x33: opcode = "INX SP"; break;
		case 0x34: opcode = "INR M"; break;
		case 0x35: opcode = "DCR M"; break;
		case 0x36:
			sprintf(fs, "MVI M, #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x37: opcode = "STC"; break;
		case 0x39: opcode = "DAD SP"; break;
		case 0x3A: 
			sprintf(fs, "LDA $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0x3B: opcode = "DCX SP"; break;
		case 0x3C: opcode = "INR A"; break;
		case 0x3D: opcode = "DCR A"; break;
		case 0x3E:
			sprintf(fs, "MVI A, #$%02X",code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0x3F: opcode = "CMC"
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
		case 0x80: opcode = "ADD B"; break;
		case 0x81: opcode = "ADD C"; break;
		case 0x82: opcode = "ADD D"; break;
		case 0x83: opcode = "ADD E"; break;
		case 0x84: opcode = "ADD H"; break;
		case 0x85: opcode = "ADD L"; break;
		case 0x86: opcode = "ADD M"; break;
		case 0x87: opcode = "ADD A"; break;
		case 0x88: opcode = "ADC B"; break;
		case 0x89: opcode = "ADC C"; break;
		case 0x8A: opcode = "ADC D"; break;
		case 0x8B: opcode = "ADC E"; break;
		case 0x8C: opcode = "ADC H"; break;
		case 0x8D: opcode = "ADC L"; break;
		case 0x8E: opcode = "ADC M"; break;
		case 0x8F: opcode = "ADC A"; break;
		case 0x90: opcode = "SUB B"; break;
		case 0x91: opcode = "SUB C"; break;
		case 0x92: opcode = "SUB D"; break;
		case 0x93: opcode = "SUB E"; break;
		case 0x94: opcode = "SUB H"; break;
		case 0x95: opcode = "SUB L"; break;
		case 0x96: opcode = "SUB M"; break;
		case 0x97: opcode = "SUB A"; break;
		case 0x98: opcode = "SBB B"; break;
		case 0x99: opcode = "SBB C"; break;
		case 0x9A: opcode = "SBB D"; break;
		case 0x9B: opcode = "SBB E"; break;
		case 0x9C: opcode = "SBB H"; break;
		case 0x9D: opcode = "SBB L"; break;
		case 0x9E: opcode = "SBB M"; break;
		case 0x9F: opcode = "SBB A"; break;
		case 0xA0: opcode = "ANA B"; break;
		case 0xA1: opcode = "ANA C"; break;
		case 0xA2: opcode = "ANA D"; break;
		case 0xA3: opcode = "ANA E"; break;
		case 0xA4: opcode = "ANA H"; break;
		case 0xA5: opcode = "ANA L"; break;
		case 0xA6: opcode = "ANA M"; break;
		case 0xA7: opcode = "ANA A"; break;
		case 0xA8: opcode = "XRA B"; break;
		case 0xA9: opcode = "XRA C"; break;
		case 0xAA: opcode = "XRA D"; break;
		case 0xAB: opcode = "XRA E"; break;
		case 0xAC: opcode = "XRA H"; break;
		case 0xAD: opcode = "XRA L"; break;
		case 0xAE: opcode = "XRA M"; break;
		case 0xAF: opcode = "XRA A"; break;
		case 0xB0: opcode = "ORA B"; break;
		case 0xB1: opcode = "ORA C"; break;
		case 0xB2: opcode = "ORA D"; break;
		case 0xB3: opcode = "ORA E"; break;
		case 0xB4: opcode = "ORA H"; break;
		case 0xB5: opcode = "ORA L"; break;
		case 0xB6: opcode = "ORA M"; break;
		case 0xB7: opcode = "ORA A"; break;
		case 0xB8: opcode = "CMP B"; break;
		case 0xB9: opcode = "CMP C"; break;
		case 0xBA: opcode = "CMP D"; break;
		case 0xBB: opcode = "CMP E"; break;
		case 0xBC: opcode = "CMP H"; break;
		case 0xBD: opcode = "CMP L"; break;
		case 0xBE: opcode = "CMP M"; break;
		case 0xBF: opcode = "CMP A"; break;
		case 0xC0: opcode = "RNZ"; break;
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
		case 0xC4: 
			sprintf(fs, "CNZ $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xC5: opcode = "PUSH B"; break;
		case 0xC6:
			sprintf(fs, "ADI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xC7: opcode = "RST 0"; break;
		case 0xC8: opcode = "RZ"; break;
		case 0xC9: opcode = "RET"; break;
		case 0xCA:
			sprintf(fs, "JZ $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xCC:
			sprintf(fs, "CZ $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xCD: 
			sprintf(fs, "CALL $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xCE:
			sprintf(fs, "ACI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xCF: opcode = "RST 1"; break;
		case 0xD0: opcode = "RNC"; break;
		case 0xD1: opcode = "POP D"; break;
		case 0xD2:
			sprintf(fs, "JNC $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xD3:
			sprintf(fs, "OUT #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xD4:
			sprintf(fs, "CNC $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xD5: opcode = "PUSH D"; break;
		case 0xD6:
			sprintf(fs, "SUI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xD7: opcode = "RST 2"; break;
		case 0xD8: opcode = "RC"; break;
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
		case 0xDC: 
			sprintf(fs, "CC $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xDE: 
			sprintf(fs, "SBI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xDF: opcode = "RST 3"; break;
		case 0xE0: opcode = "RPO"; break;
		case 0xE1: opcode = "POP H"; break;
		case 0xE2: 
			sprintf(fs, "JPO $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xE3: opcode = "XTHL"; break;
		case 0xE4: 
			sprintf(fs, "CPO $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xE5: opcode = "PUSH H"; break;
		case 0xE6: 
			sprintf(fs, "ANI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xE7: opcode = "RST 4"; break;
		case 0xE8: opcode = "RPE"; break;
		case 0xE9: opcode = "PCHL"; break;
		case 0xEA: 
			sprintf(fs, "JPE $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xEB: opcode = "XCHG"; break;
		case 0xEC: 
			sprintf(fs, "CPE $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xEE: 
			sprintf(fs, "XRI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xEF: opcode = "RST 5"; break;
		case 0xF0: opcode = "RP"; break;
		case 0xF1: opcode = "POP PSW"; break;
		case 0xF2: 
			sprintf(fs, "JP $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xF3: opcode = "DI"; break;
		case 0xF4: 
			sprintf(fs, "CP $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xF5: opcode = "PUSH PSW"; break;
		case 0xF6: 
			sprintf(fs, "ORI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xF7: opcode = "RST 6"; break;
		case 0xF8: opcode = "RM"; break;
		case 0xF9: opcode = "SPHL"; break;
		case 0xFA: 
			sprintf(fs, "JM $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xFB: opcode = "EI"; break;
		case 0xFC: 
			sprintf(fs, "CM $%02X%02X",code_buffer[pc+2], code_buffer[pc+1]);
			opcode = fs;
			opbytes = 3;
			break;
		case 0xFE:
			sprintf(fs, "CPI #$%02X", code_buffer[pc+1]);
			opcode = fs;
			opbytes = 2;
			break;
		case 0xFF: opcode = "RST 7"; break;
		default: opcode = "Not Implemented"; break;
	}
	return opcode;
}
