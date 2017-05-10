
#include <iostream>
#include <map>
#include <string>
#include <vector>


using namespace std;


typedef unsigned char byte;


class AssemblerInstruction
{
public:
	AssemblerInstruction()
	{

	}

	~AssemblerInstruction()
	{

	}

	string name;
	vector<string> tokens;
};


#pragma once
class VcpuHlInterpreter
{
public:
	VcpuHlInterpreter();
	~VcpuHlInterpreter();

	static const string VAR;
	static const string ADD;
	static const string SUBTRACT;
	static const string PRINT;

	void Build(const vector<string>& pCode);

private:    // Code
	int m_currentLine;

	vector<string> m_code;
	vector<AssemblerInstruction> m_assemblerInstructions;
	map<string, byte> m_variables;

	string Trim(const string& pInput);
	bool ParseLine(const string& pLine);
	vector<string> GetTokens(const string& pLine);
};

