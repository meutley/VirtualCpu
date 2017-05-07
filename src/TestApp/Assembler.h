
#include <string>
#include <vector>

#include "Cpu.h"


using namespace std;


#pragma once
class Assembler
{
public:
	Assembler();
	~Assembler();

public:    // Main
	vector<byte>& Assemble(const vector<string>& pCode);
	void DumpInstructions();

private:    // Code
	vector<string> m_code;

	vector<string> Split(const string& pInput);

	static const string SET;
	static const string ADD;
	static const string SUBTRACT;

private:    // Output
	vector<byte> m_instructions;
};

