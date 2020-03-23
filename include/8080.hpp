#ifndef _8080_HPP
#define _8080_HPP

#include <vector>
#include <cstdint>
#include <string>

// A class for the 8080 chip
class c8080{
	private:
		// State
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
		uint8_t e;
		uint8_t h;
		uint8_t l;
		uint16_t sp;
		uint16_t pc;
		std::vector<uint8_t> memory;
		struct ConditionCodes {
			uint8_t z;
			uint8_t s;
			uint8_t p;
			uint8_t cy;
			uint8_t ac;
		} cc;
		uint8_t int_enable;
	
	public:
		void cycle();
		void write_mem(uint16_t addr, uint8_t val);
		std::string get_state();
		c8080(const std::vector<uint8_t>& code);
};


#endif
