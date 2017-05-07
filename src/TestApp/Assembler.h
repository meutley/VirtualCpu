
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
	vector<byte> Assemble(const vector<string>& pCode);
	void DumpInstructions();


private:    // Code
	vector<string> m_code;
	unsigned int m_currentLine;

	vector<string> Split(const string& pInput);
	string Trim(const string& input);
	string CombineStrings(const vector<string>& input, const int& startFrom = 0);
	vector<string> ParseOperands(const string& input, const byte& separator = Assembler::OPERAND_SEPARATOR);

	bool CheckOperands(const vector<string>& operands, const int& expectedCount);
	bool IsRegisterValid(const string& registerName);

	static const string SET;
	static const string ADD;
	static const string SUBTRACT;

	static const byte OPERAND_SEPARATOR;


private:    // Instructions and CPU
	const byte& GetRegisterId(const string& name);

	void AddInstruction(const byte& instruction, const byte& leftOperand, const byte& rightOperand);

	bool ParseSet(const vector<string>& operands);
	bool ParseAdd(const vector<string>& operands);


private:    // Output
	vector<byte> m_instructions;
};

