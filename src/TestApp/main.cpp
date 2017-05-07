
#include <iostream>
#include <vector>

#include "Assembler.h"


using namespace std;


int main()
{
	vector<string> code;
	code.push_back("SET D0, 3");
	code.push_back("SET D1, 7");
	code.push_back("ADD D0, D1");

	Assembler* assembler = new Assembler();
	auto instructions = assembler->Assemble(code);

	Cpu* cpu = new Cpu();
	cpu->Execute(instructions);

	return 0;
}