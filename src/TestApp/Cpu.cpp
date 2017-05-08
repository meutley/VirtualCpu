
#include <iostream>

#include "Cpu.h"


const byte Cpu::MAX = 255;


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

void Cpu::Execute(vector<byte>& pProgram, const bool& pDumpOnProcessInstructions, const bool& pHaltOnOverflow, const bool& pHaltOnUnderflow)
{
	Reset();

	m_dumpOnProcessInstructions = pDumpOnProcessInstructions;

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

		// Output
		case PRINTR:
			DoPrintRegister(pProgram);
			break;
		case PRINT:
			DoPrintValue(pProgram);
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
	// Zero out all registers
	for (auto rnit = m_registerIds.begin(); rnit != m_registerIds.end(); rnit++)
	{
		this->m_registers.insert(pair<byte, byte>(*rnit, 0));
	}
}

bool Cpu::IsValidRegisterId(const byte& pRegisterId)
{
	return m_registers.find(pRegisterId) != m_registers.end();
}

string Cpu::GetRegisterName(const byte& pRegisterId)
{
	for (auto it = CpuRegisterMap.begin(); it != CpuRegisterMap.end(); it++)
	{
		if (it->second == pRegisterId)
		{
			return it->first;
		}
	}

	return "";
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
	else if (result > Cpu::MAX)
	{
		result = Cpu::MAX;
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

	if (m_dumpOnProcessInstructions)
	{
		cout << "SET instruction called..." << endl;
		Dump();
	}
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

	if (m_dumpOnProcessInstructions)
	{
		cout << "ADD instruction called..." << endl;
		Dump();
	}
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

	if (m_dumpOnProcessInstructions)
	{
		cout << "SUBTRACT instruction called..." << endl;
		Dump();
	}
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

	if (m_dumpOnProcessInstructions)
	{
		cout << "AND instruction called..." << endl;
		Dump();
	}
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

	if (m_dumpOnProcessInstructions)
	{
		cout << "OR instruction called..." << endl;
		Dump();
	}
}

/* ========== Output ========== */

void Cpu::DoPrintRegister(const vector<byte>& pProgram)
{
	byte registerId = pProgram[m_programCounter++];

	if (IsValidRegisterId(registerId))
	{
		string registerName = GetRegisterName(registerId);
		auto value = static_cast<int>(m_registers[registerId]);
		cout << "PRINTR (" << registerName << "): " << value << endl;
	}

	if (m_dumpOnProcessInstructions)
	{
		cout << endl << "PRINTR instruction called..." << endl;
		Dump();
	}
}

void Cpu::DoPrintValue(const vector<byte>& pProgram)
{
	byte programValue = pProgram[m_programCounter++];

	auto value = static_cast<int>(programValue);
	cout << "PRINT: " << value << endl;

	if (m_dumpOnProcessInstructions)
	{
		cout << endl << "PRINT instruction called..." << endl;
		Dump();
	}
}
