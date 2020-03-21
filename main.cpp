#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

#include "8080disas.hpp"

// Debug mode
const bool DEBUG = true;

int main(int argc, char* argv[]){
	
	// Flags
	bool disas_flag = false;
	
	// Rom
	std::string rom_name = "";

	// Parse command line arguments
	for(int i = 1; i < argc; ++i){
		if(std::string(argv[i]) == "-d"){
			disas_flag = true;
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
	std::vector<unsigned char> code_buffer(std::istreambuf_iterator<char>(rf), {});

	// Execute or disassembly
	if(disas_flag){
		unsigned int pc = 0;
		while(pc < code_buffer.size()){
			pc += disas8080op(code_buffer, pc);
		}
	}else{
		// Run
		printf("Execution not yet implemented.\n");
	}

	return 0;
}
