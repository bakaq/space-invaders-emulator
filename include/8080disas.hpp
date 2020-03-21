#ifndef _8080DISAS_HPP
#define _8080DISAS_HPP

#include <vector>
#include <string>

// Disassembles and 8080 instruction, returning the the
// corresponding assembly and modifying the opbytes if needed
std::string disas8080op(const std::vector<unsigned char> &code_buffer, int pc, int &opbytes);

// Disassembles an 8080 instruction, printing to standard output,
// and returns the number of bytes it used
int disas8080opf(const std::vector<unsigned char> &code_buffer, int pc);



#endif
