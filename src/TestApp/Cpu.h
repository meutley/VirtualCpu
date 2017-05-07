
#include <string>
#include <map>
#include <vector>


using namespace std;


typedef unsigned char byte;


#pragma once
class Cpu
{
public:
	Cpu();
	~Cpu();

	void Reset();

	enum Instructions
	{
		SET = 0,
		ADD,
		SUBTRACT
	};

	enum Registers
	{
		D0 = 0,
		D1
	};

	void Execute(vector<byte>& pProgram, const bool& pHaltOnOverflow = true, const bool& pHaltOnUnderflow = true);
	void Dump();

private:    // Registers
	byte m_instructionRegister;

	vector<byte> m_registerIds;
	map<byte, byte> m_registers;

	void ResetRegisters();
	bool IsValidRegisterId(const byte& registerId);

private:    // Program
	byte m_programCounter;

	bool m_isOverflow;
	bool m_isUnderflow;
	bool m_haltOnOverflow;
	bool m_haltOnUnderflow;

	void ResetProgram();

private:    // Instruction Processors
	void DoSet(const vector<byte>& pProgram);
	void DoAdd(const vector<byte>& pProgram);
};


static const map<string, byte> CpuRegisterMap
{
	pair<string, byte>("D0", Cpu::D0),
	pair<string, byte>("D1", Cpu::D1)
};

