
#include <iostream>

#include "Cpu.h"


Cpu::Cpu()
{
	Reset();
}

Cpu::~Cpu()
{

}


void Cpu::Reset()
{
	ResetRegisters();
	ResetProgram();
}


/* ========== Main ==========*/

void Cpu::Execute(vector<byte>& pProgram, const bool& pHaltOnOverflow, const bool& pHaltOnUnderflow)
{
	Reset();

	m_haltOnOverflow = pHaltOnOverflow;
	m_haltOnUnderflow = pHaltOnUnderflow;

	while (m_programCounter < pProgram.size())
	{
		m_instructionRegister = pProgram[m_programCounter];
		m_programCounter++;

		switch (m_instructionRegister)
		{
		// Assignment
		case SET:
			DoSet(pProgram);
			break;

		// Arithmetic
		case ADD:
			DoAdd(pProgram);
			break;
		case SUBTRACT:
			DoSubtract(pProgram);
			break;

		// Bitwise
		case AND:
			DoAnd(pProgram);
			break;

		case OR:
			DoOr(pProgram);
			break;
		}

		if (m_isOverflow && m_haltOnOverflow)
		{
			cout << "Overflow occurred; program halted" << endl << endl;
			return;
		}

		if (m_isUnderflow && m_haltOnUnderflow)
		{
			cout << "Underflow occurred; program halted" << endl << endl;
			return;
		}
	}
}

void Cpu::Dump()
{
	cout << "=============== REGISTER DUMP ===============" << endl << endl;
	for (auto it = CpuRegisterMap.begin(); it != CpuRegisterMap.end(); it++)
	{
		auto reg = m_registers.find(it->second);
		if (reg != m_registers.end())
		{
			cout << "[" << it->first << "] : " << static_cast<int>(reg->second) << endl;
		}
	}
	cout << endl << "=============================================" << endl << endl << endl;
}


/* ========== Registers ========== */

void Cpu::ResetRegisters()
{
	m_registerIds.clear();
	m_registerIds.push_back(Cpu::Register::D0);
	m_registerIds.push_back(Cpu::Register::D1);

	m_registers.clear();
	for (auto rnit = m_registerIds.begin(); rnit != m_registerIds.end(); rnit++)
	{
		this->m_registers.insert(pair<byte, byte>(*rnit, 0));
	}
}

bool Cpu::IsValidRegisterId(const byte& pRegisterId)
{
	return m_registers.find(pRegisterId) != m_registers.end();
}


/* ========== Program ========== */

void Cpu::ResetProgram()
{
	m_programCounter = 0;
	m_isOverflow = false;
	m_isUnderflow = false;
	m_haltOnOverflow = false;
	m_haltOnUnderflow = false;
}

void Cpu::CheckArithmeticResult(int& result)
{
	if (result < 0)
	{
		result = 0;
		m_isUnderflow = true;
	}
	else if (result > 255)
	{
		result = 255;
		m_isOverflow = true;
	}
}

/* ========== Assignment ========== */

void Cpu::DoSet(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	if (m_registers.find(destinationRegisterId) != m_registers.end())
	{
		m_registers[destinationRegisterId] = pProgram[m_programCounter++];
	}

	cout << "SET instruction called..." << endl;
	Dump();
}

/* ========== Arithmetic ========== */

void Cpu::DoAdd(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	byte sourceRegisterId = pProgram[m_programCounter++];

	if (IsValidRegisterId(destinationRegisterId) && IsValidRegisterId(sourceRegisterId))
	{
		auto left = static_cast<int>(m_registers[destinationRegisterId]);
		auto right = static_cast<int>(m_registers[sourceRegisterId]);

		int sum = left + right;
		CheckArithmeticResult(sum);
		
		m_registers[destinationRegisterId] = left + right;
	}

	cout << "ADD instruction called..." << endl;
	Dump();
}

void Cpu::DoSubtract(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	byte sourceRegisterId = pProgram[m_programCounter++];

	if (IsValidRegisterId(destinationRegisterId) && IsValidRegisterId(sourceRegisterId))
	{
		auto left = static_cast<int>(m_registers[destinationRegisterId]);
		auto right = static_cast<int>(m_registers[sourceRegisterId]);

		int difference = left - right;
		CheckArithmeticResult(difference);

		m_registers[destinationRegisterId] = left - right;
	}

	cout << "SUBTRACT instruction called..." << endl;
	Dump();
}

/* ========== Bitwise ========== */

void Cpu::DoAnd(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	byte sourceRegisterId = pProgram[m_programCounter++];

	if (IsValidRegisterId(destinationRegisterId) && IsValidRegisterId(sourceRegisterId))
	{
		auto left = static_cast<int>(m_registers[destinationRegisterId]);
		auto right = static_cast<int>(m_registers[sourceRegisterId]);

		int result = left & right;
		CheckArithmeticResult(result);

		m_registers[destinationRegisterId] = left & right;
	}

	cout << "AND instruction called..." << endl;
	Dump();
}

void Cpu::DoOr(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	byte sourceRegisterId = pProgram[m_programCounter++];

	if (IsValidRegisterId(destinationRegisterId) && IsValidRegisterId(sourceRegisterId))
	{
		auto left = static_cast<int>(m_registers[destinationRegisterId]);
		auto right = static_cast<int>(m_registers[sourceRegisterId]);

		int result = left | right;
		CheckArithmeticResult(result);

		m_registers[destinationRegisterId] = left | right;
	}

	cout << "OR instruction called..." << endl;
	Dump();
}
