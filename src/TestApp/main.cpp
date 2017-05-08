
#include <fstream>
#include <iostream>
#include <vector>

#include "Assembler.h"


using namespace std;


const string SampleProgramPath = "../TestApp/SampleProgram.txt";

int main()
{
	cout << "Loading sample program at " << SampleProgramPath << "..." << endl << endl;

	ifstream infile(SampleProgramPath);
	// Make sure file is open
	if (!infile.is_open())
	{
		cout << "Failed to open sample program: " << SampleProgramPath << endl << endl;
		return 99;
	}

	// Read the sample program into memory
	vector<string> code;

	string line;
	while (getline(infile, line))
	{
		code.push_back(line);
	}

	cout << "Assembling sample program..." << endl << endl;

	Assembler assembler;
	auto instructions = assembler.Assemble(code);

	if (instructions.size() == 0)
	{
		cout << endl << "No instructions generated. Program assembly failed?" << endl << endl;
	}
	else
	{
		cout << "Pushing assembled program to virtual CPU..." << endl << endl;

		Cpu cpu;
		cpu.Execute(instructions);

		cout << "Program execution complete..." << endl << endl;
	}

	return 0;
}