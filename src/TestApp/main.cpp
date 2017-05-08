
#include <iostream>
#include <vector>

#include "Assembler.h"


using namespace std;


int main()
{
	vector<string> code;
	// Add
	code.push_back("SET D0, 3");
	code.push_back("SET D1, 7");
	code.push_back("ADD D0, D1");

	// Subtract
	code.push_back("SET D1, 5");
	code.push_back("SUBTRACT D0, D1");

	// Bitwise OR
	code.push_back("SET D1, 2");
	code.push_back("OR D0, D1");

	Assembler assembler;
	auto instructions = assembler.Assemble(code);

	Cpu cpu;
	cpu.Execute(instructions);

	return 0;
}