
#include <iostream>
#include <sstream>

#include "Assembler.h"


const string Assembler::SET = string("SET");
const string Assembler::ADD = string("ADD");
const string Assembler::SUBTRACT = string("SUBTRACT");


Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}


/* ========== Main ========== */

vector<byte>& Assembler::Assemble(const vector<string>& pCode)
{
	for (auto it = pCode.begin(); it != pCode.end(); it++)
	{
		vector<string> line = this->Split(*it);
		
		auto opCode = line[0];
		if (opCode == Assembler::SET)
		{
			if (line.size() != 3)
			{
				cout << "ERROR: Expected opcode and two operands, but got " << line.size() << " :: " << *it << endl;
				return vector<byte>();
			}

			auto leftOperand = line[1].substr(0, line[1].find_first_of(','));
			auto rightOperand = stoi(line[2]);

			m_instructions.push_back(Cpu::SET);
			m_instructions.push_back(CpuRegisterMap.find(leftOperand)->second);
			m_instructions.push_back(rightOperand);
		}
		else if (opCode == Assembler::ADD)
		{
			if (line.size() != 3)
			{
				cout << "ERROR: Expected opcode and two operands, but got " << line.size() << " :: " << *it << endl;
				return vector<byte>();
			}

			auto leftOperand = line[1].substr(0, line[1].find_first_of(','));
			auto rightOperand = line[2];

			m_instructions.push_back(Cpu::ADD);
			m_instructions.push_back(CpuRegisterMap.find(leftOperand)->second);
			m_instructions.push_back(CpuRegisterMap.find(rightOperand)->second);
		}
	}

	DumpInstructions();

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
