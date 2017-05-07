
#include <iostream>
#include <sstream>

#include "Assembler.h"


const string Assembler::SET = string("SET");
const string Assembler::ADD = string("ADD");
const string Assembler::SUBTRACT = string("SUBTRACT");

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
	m_currentLine = 0;
	for (auto it = pCode.begin(); it != pCode.end(); it++)
	{
		auto line = this->Split(Trim(*it));
		auto combinedOperands = CombineStrings(line, 1);
		auto operands = ParseOperands(combinedOperands);
		
		auto opCode = line[0];
		if (opCode == Assembler::SET)
		{
			if (!ParseSet(operands))
			{
				return vector<byte>();
			}
		}
		else if (opCode == Assembler::ADD)
		{
			if (!ParseAdd(operands))
			{
				return vector<byte>();
			}
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
		cout << "ERROR: Register name " << registerName << " not found at line " << (m_currentLine + 1) << endl;
		return false;
	}

	return true;
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

bool Assembler::ParseSet(const vector<string>& operands)
{
	if (CheckOperands(operands, 2))
	{
		try
		{
			auto leftOperand = operands[0];
			auto rightOperand = stoi(operands[1]);

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
			return false;
		}
		catch (out_of_range outOfRange)
		{
			cout << "ERROR: INVALID OPERAND" << endl;
			cout << "   Right operand out of range(max = 255), current = " << operands[1] << endl;
			cout << "   LINE: " << (m_currentLine + 1) << endl;
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
	if (CheckOperands(operands, 2))
	{
		auto leftOperand = operands[0];
		auto rightOperand = operands[1];

		if (IsRegisterValid(leftOperand) && IsRegisterValid(rightOperand))
		{
			AddInstruction(Cpu::ADD, GetRegisterId(leftOperand), GetRegisterId(rightOperand));
		}
	}
	else
	{
		return false;
	}

	return true;
}
