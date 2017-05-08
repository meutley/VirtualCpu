
#include <iostream>
#include <sstream>

#include "Assembler.h"


const string Assembler::SET = string("SET");
const string Assembler::ADD = string("ADD");
const string Assembler::SUBTRACT = string("SUBTRACT");
const string Assembler::AND = string("AND");
const string Assembler::OR = string("OR");

const byte Assembler::OPERAND_SEPARATOR = ',';


Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}


/* ========== Main ========== */

vector<byte> Assembler::Assemble(const vector<string>& pCode)
{
	m_code = pCode;
	m_currentLine = 0;
	for (auto it = pCode.begin(); it != pCode.end(); it++)
	{
		auto line = this->Split(Trim(*it));
		if (line[0].length() == 0 || line[0][0] == '#')
		{
			m_currentLine++;
			continue;
		}

		auto combinedOperands = CombineStrings(line, 1);
		auto operands = ParseOperands(combinedOperands);
		
		auto opCode = line[0];
		// Assignment
		if (opCode == Assembler::SET)
		{
			if (!ParseSet(operands))
			{
				return vector<byte>();
			}
		}
		// Arithmetic
		else if (opCode == Assembler::ADD)
		{
			if (!ParseAdd(operands))
			{
				return vector<byte>();
			}
		}
		else if (opCode == Assembler::SUBTRACT)
		{
			if (!ParseSubtract(operands))
			{
				return vector<byte>();
			}
		}
		// Bitwise
		else if (opCode == Assembler::AND)
		{
			if (!ParseAnd(operands))
			{
				return vector<byte>();
			}
		}
		else if (opCode == Assembler::OR)
		{
			if (!ParseOr(operands))
			{
				return vector<byte>();
			}
		}
		else
		{
			cout << endl << "ERROR: OpCode not recognized." << endl;
			cout << "   LINE: " << m_currentLine << endl;
			cout << "      >> " << m_code[m_currentLine] << endl;

			return vector<byte>();
		}

		m_currentLine++;
	}

	return m_instructions;
}

void Assembler::DumpInstructions()
{
	for (auto it = m_instructions.begin(); it != m_instructions.end(); it++)
	{
		cout << (int)(*it) << endl;
	}
}


/* ========== Code ========== */

vector<string> Assembler::Split(const string& pInput)
{
	vector<string> result;

	string temp;
	istringstream stream(pInput, istringstream::in);
	while (stream.good())
	{
		stream >> temp;
		result.push_back(temp);
	}
	
	return result;
}

string Assembler::Trim(const string& input)
{
	if (input.length() == 0)    // Empty, nothing to do
	{
		return input;
	}

	// Trim trailing and leading spaces
	string output = input;
	auto leftPosition = input.find_first_not_of(' ');
	auto rightPosition = input.find_last_not_of(' ');
	
	// Return untouched input string if no spaces found to trim
	if (leftPosition > input.length() && rightPosition > input.length())
	{
		return input;
	}
	
	return output.substr(leftPosition, rightPosition + 1);
}

string Assembler::CombineStrings(const vector<string>& input, const int& startFrom)
{
	string result;

	if (startFrom > input.size())
	{
		return result;
	}

	for (auto it = input.begin() + startFrom; it != input.end(); it++)
	{
		result.append(*it);
	}

	return result;
}

vector<string> Assembler::ParseOperands(const string& input, const byte& separator)
{
	vector<string> result;

	auto trimmed = Trim(input);
	if (trimmed.length() == 0)
	{
		return result;
	}

	string currentOperand;
	int index = 0;
	while (true)
	{
		if (index == trimmed.length())    // End of input
		{
			if (currentOperand.length() > 0 && Trim(currentOperand).length() > 0)
			{
				result.push_back(currentOperand);
			}

			break;
		}

		if (trimmed[0] == '#')    // Ignore comments
		{
			continue;
		}

		while (trimmed[index] == ' ')    // Skip whitespace
		{
			index++;
			continue;
		}

		if (trimmed[index] == separator)    // Operand parsed; look for next operand
		{
			if (Trim(currentOperand).length() > 0)
			{
				result.push_back(currentOperand);
			}

			currentOperand = "";
			index++;
			continue;
		}

		currentOperand += trimmed[index];
		index++;
	}
	
	return result;
}

bool Assembler::CheckOperands(const vector<string>& operands, const int& expectedCount)
{
	if (operands.size() != expectedCount)
	{
		cout << "ERROR: Expected opcode and " << expectedCount << " operand(s), but got " << operands.size() << " at Line " << (m_currentLine + 1) << endl;
		return false;
	}

	return true;
}

bool Assembler::IsRegisterValid(const string& registerName)
{
	if (CpuRegisterMap.find(registerName) == CpuRegisterMap.end())
	{
		cout << "ERROR: Register \"" << registerName << "\" not found.";
		cout << "   LINE: " << (m_currentLine + 1) << endl;
		cout << "      >> " << m_code[m_currentLine] << endl;
		return false;
	}

	return true;
}

byte Assembler::ToByte(int value)
{
	if (value >= 0 && value <= 255)
	{
		return (byte)value;
	}

	throw out_of_range("value must be between 0 and 255");
}

const byte& Assembler::GetRegisterId(const string& name)
{
	if (IsRegisterValid(name))
	{
		return CpuRegisterMap.find(name)->second;
	}

	return byte();
}

void Assembler::AddInstruction(const byte& instruction, const byte& leftOperand, const byte& rightOperand)
{
	m_instructions.push_back(instruction);
	m_instructions.push_back(leftOperand);
	m_instructions.push_back(rightOperand);
}

/* ========== Assignment ========== */

bool Assembler::ParseSet(const vector<string>& operands)
{
	if (CheckOperands(operands, 2))
	{
		try
		{
			auto leftOperand = operands[0];
			auto rightOperand = ToByte(stoi(operands[1]));

			if (IsRegisterValid(leftOperand))
			{
				AddInstruction(Cpu::SET, GetRegisterId(leftOperand), rightOperand);
			}
			else
			{
				return false;
			}
		}
		catch (invalid_argument invalidArg)
		{
			cout << "ERROR: INVALID OPERAND" << endl;
			cout << "   Right operand must be a valid integer, current = " << operands[1] << endl;
			cout << "   LINE: " << (m_currentLine + 1) << endl;
			cout << "      >> " << m_code[m_currentLine];
			return false;
		}
		catch (out_of_range outOfRange)
		{
			cout << "ERROR: INVALID OPERAND" << endl;
			cout << "   Right operand out of range (min = 0, max = 255), current = " << operands[1] << endl;
			cout << "   LINE: " << (m_currentLine + 1) << endl;
			cout << "      >> " << m_code[m_currentLine] << endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

/* ========== Arithmetic ========== */

bool Assembler::ParseArithmetic(Cpu::Instruction instruction, const vector<string>& operands)
{
	if (CheckOperands(operands, 2))
	{
		auto leftOperand = operands[0];
		auto rightOperand = operands[1];

		if (IsRegisterValid(leftOperand) && IsRegisterValid(rightOperand))
		{
			AddInstruction(instruction, GetRegisterId(leftOperand), GetRegisterId(rightOperand));
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool Assembler::ParseAdd(const vector<string>& operands)
{
	return ParseArithmetic(Cpu::Instruction::ADD, operands);
}

bool Assembler::ParseSubtract(const vector<string>& operands)
{
	return ParseArithmetic(Cpu::Instruction::SUBTRACT, operands);
}

/* ========== Bitwise ========== */

bool Assembler::ParseAnd(const vector<string>& operands)
{
	return ParseArithmetic(Cpu::Instruction::AND, operands);
}

bool Assembler::ParseOr(const vector<string>& operands)
{
	return ParseArithmetic(Cpu::Instruction::OR, operands);
}
