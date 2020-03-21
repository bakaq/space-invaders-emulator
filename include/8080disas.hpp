#ifndef _8080DISAS_HPP
#define _8080DISAS_HPP

#include <vector>

// Disassembles an 8080 operation and returns the number of bytes
// it used
int disas8080op(std::vector<unsigned char> code_buffer, int pc);

#endif
