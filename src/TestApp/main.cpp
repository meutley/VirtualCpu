
#include <iostream>
#include <vector>

#include "Cpu.h"


using namespace std;


int main()
{
	Cpu* cpu = new Cpu();

	vector<byte> program;
	program.push_back(Cpu::SET);
	program.push_back(Cpu::D0);
	program.push_back(17);
	program.push_back(Cpu::SET);
	program.push_back(Cpu::D1);
	program.push_back(5);
	program.push_back(Cpu::ADD);
	program.push_back(Cpu::D0);
	program.push_back(Cpu::D1);

	cpu->Execute(program);

	return 0;
}