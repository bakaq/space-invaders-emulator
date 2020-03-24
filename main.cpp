#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>
#include <chrono>

#include "8080.hpp"
#include "8080disas.hpp"


// Debug mode
bool DEBUG = false;


// Screen size
const int SCREEN_W = 640;
const int SCREEN_H = 480;

// Shift resister
uint16_t shift_register = 0x0000;
uint8_t shift_offset = 0x00;

// Functions
void execute(const std::vector<uint8_t> &code);
void machine_IN(c8080 &chip, int port, uint8_t input);
void machine_OUT(const c8080 &chip, int port);
double time();

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
	
	// SDL setup
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	uint32_t *pixels = new uint32_t[SCREEN_W*SCREEN_H];

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
	SDL_Event e;
	
	c8080 chip(code);
	bool running = true;
	double last_int = time();
	while(running){
		
		uint8_t input = 0;
		// Get input
		while(SDL_PollEvent(&e) != 0){
			switch(e.type){
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_LEFT: input |= 0x20; break;
						case SDLK_RIGHT: input |= 0x40; break;
						case SDLK_SPACE: input |= 0x10; break;
						case SDLK_RETURN: input |= 0x04; break;
						case SDLK_c: input |= 0x01; break;
					}
					break;
				case SDL_KEYUP:
					switch(e.key.keysym.sym){
						case SDLK_LEFT: input &= 0xDF; break;
						case SDLK_RIGHT: input &= 0xBF; break;
						case SDLK_SPACE: input &= 0xEF; break;
						case SDLK_RETURN: input &= 0xFB; break;
						case SDLK_c: input &= 0xFE; break;
					}
					break;
			}
		}
		
		// Emulate cycle
		uint8_t op = chip.memory[chip.pc];
		if(op == 0xDB){ // IN
			uint8_t port = chip.memory[chip.pc + 1];
			machine_IN(chip, port, input);
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
		
		// Interrupt
		if((time() - last_int > 1.0/60.0) && (chip.int_enable)){
			printf("INTERRUPT!!! Elapsed time: %f\n", time() - last_int);
			chip.gen_interrupt(2);
			last_int = time();
		}

		// Draw screen
		// TODO: Draw screen
		/*
		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_W*sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		*/
	}

	
	// Cleanup
	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void machine_IN(c8080 &chip, int port, uint8_t input){
	switch(port){
		case 1:
			chip.a = 0x01;//input;
			break;
		case 2:
			chip.a = 0x00;
			break;
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

double time(){
	return static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count())/1000000000;
}
