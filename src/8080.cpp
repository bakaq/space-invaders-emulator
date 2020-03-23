#include <vector>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <string>

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

std::string c8080::get_state(){
	char buffer[100];
	sprintf(buffer, "A: %02X BC: %04X DE: %04X HL: %04X\n", this->a, (this->b << 8) | this->c, (this->d << 8) | this->e, (this->h << 8) | this->l);
	return std::string(buffer);
}


void c8080::write_mem(uint16_t addr, uint8_t val){
	if(addr >= 0x0800){
		this->memory[addr] = val;
	}
}

void c8080::cycle(){
	std::vector<uint8_t>& mem = this->memory;
	uint16_t& pc = this->pc;
	uint16_t& sp = this->sp;

	uint8_t op = mem[pc];
	disas8080opf(mem, pc);
	switch(op){
		case 0x00: break; // NOP
		case 0x01: // LXI B, D16
			this->b = mem[pc+2];
			this->c = mem[pc+1];
			pc += 2;
			break;
		case 0x06: this->b = mem[pc+1];pc += 1;break; // MVI B, D8
		case 0x05: // DCR B
		{
			uint16_t ans = this->b - 1; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->b = ans & 0xFF;
			break;
		}
		case 0x09: // DAD B
		{	
			uint32_t ans = ((this->h << 8) | this->l) + ((this->b << 8) | this->c);
			this->cc.cy = (ans > 0xFFFF);
			this->h = (ans & 0xFF00) >> 8;
			this->l = (ans & 0x00FF);
			break;
		}
		case 0x0D: // DCR C
		{
			uint16_t ans = this->c - 1; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->c = ans & 0xFF;
			break;
		}
		case 0x0E: this->c = mem[pc+1]; pc+=1; break; //MVI C, D8
		case 0x0F: // RRC
			this->cc.cy = this->a & 0x01;
			this->a = ((this->a & 0x01) << 7) | (this->a >> 1);
			break;
		case 0x11: // LXI D, D16
			this->d = mem[pc+2];
			this->e = mem[pc+1];
			pc += 2;
			break;
		case 0x13: // INX D
			this->d = ((((this->d << 8) | this->e) + 1) & 0xFF00) >> 8;
			this->e = (((this->d << 8) | this->e) + 1) & 0x00FF;
			break;
		case 0x14: // INR D
		{
			uint16_t ans = this->d + 1; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->cc.cy = (ans > 0xFF);
			this->d = ans & 0xFF;
			break;
		}
		case 0x19: // DAD D
		{	
			uint32_t ans = ((this->h << 8) | this->l) + ((this->d << 8) | this->e);
			this->cc.cy = (ans > 0xFFFF);
			this->h = (ans & 0xFF00) >> 8;
			this->l = (ans & 0x00FF);
			break;
		}
		case 0x1A: // LDAX D
			this->a = mem[(this->d << 8) | this->e];
			break;
		case 0x21: // LXI H, D16
			this->h = mem[pc+2];
			this->l = mem[pc+1];
			pc += 2;
			break;
		case 0x23: // INX H
			this->h = ((((this->h << 8) | this->l) + 1) & 0xFF00) >> 8;
			this->l = (((this->h << 8) | this->l) + 1) & 0x00FF;
			break;
		case 0x26: this->h = mem[pc+1]; pc+=1; break; // MVI H, D8
		case 0x29: // DAD H
		{	
			uint32_t ans = ((this->h << 8) | this->l) + ((this->h << 8) | this->l);
			this->cc.cy = (ans > 0xFFFF);
			this->h = (ans & 0xFF00) >> 8;
			this->l = (ans & 0x00FF);
			break;
		}
		case 0x31: // LXI SP, D16
			sp = ((mem[pc+2] << 8) | mem[pc+1]);
			pc += 2;
			break;
		case 0x32: // STA adr
			write_mem((mem[pc+2] << 8) | mem[pc+1], this->a);
			pc += 2;
			break;
		case 0x36: // MVI M, D8
			write_mem((this->h << 8) | this->l, mem[pc+1]);
			pc += 1;
			break;
		case 0x39: // DAD SP
		{	
			uint32_t ans = ((this->h << 8) | this->l) + sp;
			this->cc.cy = (ans > 0xFFFF);
			this->h = (ans & 0xFF00) >> 8;
			this->l = (ans & 0x00FF);
			break;
		}
		case 0x3A: // LDA adr
			this->a = mem[mem[pc+2]<<8 | mem[pc+1]];
			pc += 2;
			break;
		case 0x3D: // DCR A
		{
			uint16_t ans = this->a - 1; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->a = ans & 0xFF;
			break;
		}
		case 0x3E: this->a = mem[pc+1]; pc+=1; break; // MVI A, D8
		case 0x46: this->b = mem[(this->h << 8) | this->l]; break; // MOV B, M
		case 0x4F: this->c = this->a; break; // MOV C, A
		case 0x56: this->d = mem[(this->h << 8) | this->l]; break; // MOV D, M
		case 0x5C: this->e = this->h; break; // MOV E, H
		case 0x5E: this->e = mem[(this->h << 8) | this->l]; break; // MOV E, M
		case 0x66: this->h = mem[(this->h << 8) | this->l]; break; // MOV H, M
		case 0x6F: this->l = this->a; break; // MOV L, A
		case 0x77: // MOV M, A 
			write_mem((this->h << 8) | this->l, this->a);
			break;
		case 0x79: this->a = this->c; break; // MOV A, C
		case 0x7A: this->a = this->d; break; // MOV A, D
		case 0x7B: this->a = this->e; break; // MOV A, E
		case 0x7C: this->a = this->h; break; // MOV A, H
		case 0x7E: this->a = mem[(this->h << 8) | this->l]; break; // MOV A, M
		case 0xA7: this->a = this->a & this->a; break; // ANA A
		case 0xAF: this->a = this->a ^ this->a; break; // XRA A
		case 0xB0: // ORA B
		{
			uint16_t ans = this->a | this->b; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->cc.cy = (ans > 0xFF);
			this->b = ans & 0xFF;
			break;
		}
		case 0xC1: // POP B
			this->c = mem[sp];
			this->b = mem[sp+1];
			sp += 2;
			break;
		case 0xC2: // JNZ adr
			if(!this->cc.z)
				pc = ((mem[pc+2] << 8) | mem[pc+1]) - 1;
			else
				pc += 2;
			break;
		case 0xC3: // JMP adr
			pc = ((mem[pc+2] << 8) | mem[pc+1]) - 1; 
			break;
		case 0xC5: // PUSH B
			write_mem(sp-2, this->c);
			write_mem(sp-1, this->b);
			sp -= 2;
			break;
		case 0xC6: // ADI D8
		{
			uint16_t ans = this->a + mem[pc+1]; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->cc.cy = (ans > 0xFF);
			this->a = ans & 0xFF;
			break;
		}
		case 0xC8: // RZ
			if(this->cc.z){
				pc = (mem[sp+1] << 8) | mem[sp];
				sp += 2;
			}
			break;
		case 0xC9: // RET
			pc = ((mem[sp+1] << 8) | mem[sp]);
			sp += 2;
			break;
		case 0xCA: // JZ adr
			if(this->cc.z)
				pc = (mem[pc+2] << 8) | mem[pc+1];
			else
				pc += 2;
			break;
		case 0XCD: // CALL adr
			write_mem(sp-1, ((pc+2) & 0xFF00) >> 8);
			write_mem(sp-2, ((pc+2) & 0x00FF));
			sp = sp - 2;
			pc = ((mem[pc+2] << 8) | mem[pc+1]) - 1;
			break;
		case 0xD0: // RNC
			if(!this->cc.cy){
				pc = (mem[sp+1] << 8) | mem[sp];
				sp += 2;
			}
			break;
		case 0xD1: //POP D
			this->e = mem[sp];
			this->d = mem[sp+1];
			sp += 2;
			break;
		case 0xD3: pc+=1; break; // OUT D8 TODO
		case 0xD5: // PUSH D
			write_mem(sp-2, this->e);
			write_mem(sp-1, this->d);
			sp -= 2;
			break;
		case 0xE1: // POP H
			this->l = mem[sp];
			this->h = mem[sp+1];
			sp += 2;
			break;
		case 0xE3: // XTHL
		{
			uint8_t aux;
			aux = this->h;
			this->h = mem[sp+1];
			write_mem(sp+1, aux);
			aux = this->l;
			this->l = mem[sp];
			write_mem(sp, aux);
			break;
		}
		case 0xE5: // PUSH H
			write_mem(sp-2, this->l);
			write_mem(sp-1, this->h);
			sp -= 2;
			break;
		case 0xE6: // ANI D8
		{
			uint16_t ans = a & mem[pc+1]; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->cc.cy = (ans > 0xFF);
			this->a = ans & 0xFF;
			pc += 1;
			break;
		}
		case 0xE9: // PCHL
			pc = ((mem[pc+2] << 8) | mem[pc+1]) - 1;
			break;
		case 0xEB: // XCHG
		{
			uint8_t aux;
			aux = this->h;
			this->h = this->d;
			this->d = aux;
			aux = this->l;
			this->l = this->e;
			this->e = aux;
			break;
		}
		case 0xF1: // POP PSW
		{
			uint8_t aux = mem[sp];
			this->cc.z = aux & 0x01;
			this->cc.s = aux & 0x02 >> 1;
			this->cc.p = aux & 0x04 >> 2;
			this->cc.cy = aux & 0x08 >> 3;
			this-> a = mem[sp+1];
			sp += 2;
			break;
		}
		case 0xF5: // PUSH PSW
			// Couldn't find how the flags were stored
			// So I'll just make up something
			// It shouldn't cause problems
			// 0000 cy p s z
			write_mem(sp-2, (this->cc.cy << 3) | (this->cc.p << 2) | (this->cc.s << 1) | this->cc.z);
			write_mem(sp-1, this->a);
			sp -= 2;
			break;
		case 0xFB: this->int_enable = 1; break; // EI
		case 0xFE: // CPI D8
		{
			uint16_t ans = a - mem[pc+1]; 
			this->cc.z = ((ans & 0xFF) == 0);
			this->cc.s = ((ans & 0x80) >> 7 == 1);
			this->cc.p = ((ans & 0x01) == 0);
			this->cc.cy = (ans > 0xFF);
			pc += 1;
			break;
		}
		case 0xFF: // RST 7
			write_mem(sp-1, (pc & 0xFF00) >> 8);
			write_mem(sp-2, (pc & 0x00FF));
			sp = sp - 2;
			pc = 0x38 - 1;
			break;	
		default:
			printf("Instruction not implemented!\n");
			exit(1);
	}
	pc += 1;
}
