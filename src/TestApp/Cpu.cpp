
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
		case SET:
			DoSet(pProgram);
			break;
		case ADD:
			DoAdd(pProgram);
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
	for (auto it = m_registers.begin(); it != m_registers.end(); it++)
	{
		cout << "[D" << static_cast<int>(it->first) << "] : " << static_cast<int>(it->second) << endl;
	}
	cout << endl << "=============================================" << endl << endl << endl;
}


/* ========== Registers ========== */

void Cpu::ResetRegisters()
{
	m_registerIds.clear();
	m_registerIds.push_back(Cpu::Registers::D0);
	m_registerIds.push_back(Cpu::Registers::D1);

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

void Cpu::DoAdd(const vector<byte>& pProgram)
{
	byte destinationRegisterId = pProgram[m_programCounter++];
	byte sourceRegisterId = pProgram[m_programCounter++];

	if (IsValidRegisterId(destinationRegisterId) && IsValidRegisterId(sourceRegisterId))
	{
		auto left = static_cast<int>(m_registers[destinationRegisterId]);
		auto right = static_cast<int>(m_registers[sourceRegisterId]);

		int sum = left + right;
		if (sum > 255)
		{
			m_isOverflow = true;
		}
		
		m_registers[destinationRegisterId] = left + right;
	}

	cout << "ADD instruction called..." << endl;
	Dump();
}
