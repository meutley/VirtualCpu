
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
	bool ParseByte(const string& value);

	bool CheckOperands(const vector<string>& operands, const int& expectedCount);
	bool IsRegisterValid(const string& registerName);
	byte ToByte(int value);

	// Instruction names
	static const string SET;
	static const string ADD;
	static const string SUBTRACT;
	static const string AND;
	static const string OR;
	static const string PRINT;

	static const byte OPERAND_SEPARATOR;


private:    // Instructions and CPU
	const byte& GetRegisterId(const string& name);

	void AddInstruction(const byte& instruction, const byte& operand);
	void AddInstruction(const byte& instruction, const byte& leftOperand, const byte& rightOperand);

	// Assignment
	bool ParseSet(const vector<string>& operands);
	bool ParseSetFromRegister(const string& destinationRegisterName, const string& sourceRegisterName);
	bool ParseSetFromRawValue(const string& destinationRegisterName, const string& rawValue);

	// Arithmetic
	bool ParseArithmetic(Cpu::Instruction instruction, const vector<string>& operands);
	bool ParseAdd(const vector<string>& operands);
	bool ParseSubtract(const vector<string>& operands);

	// Bitwise
	bool ParseAnd(const vector<string>& operands);
	bool ParseOr(const vector<string>& operands);

	// Output
	bool ParsePrint(const vector<string>& operands);
	bool ParsePrintRegister(const string& registerName);
	bool ParsePrintRawValue(const string& rawValue);


private:    // Output
	vector<byte> m_instructions;
};

