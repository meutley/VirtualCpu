
#include "VcpuHlInterpreter.h"


const string VcpuHlInterpreter::VAR = string("VAR");
const string VcpuHlInterpreter::ADD = string("ADD");
const string VcpuHlInterpreter::SUBTRACT = string("SUBTRACT");
const string VcpuHlInterpreter::PRINT = string("PRINT");


VcpuHlInterpreter::VcpuHlInterpreter()
{
}

VcpuHlInterpreter::~VcpuHlInterpreter()
{
}


/* ========== Code ========== */

void VcpuHlInterpreter::Build(const vector<string>& pCode)
{
	m_code = pCode;
	for (auto it = m_code.begin(); it != m_code.end(); it++)
	{
		string line = Trim(*it);
		if (!ParseLine(line))
		{
			return;
		}

		m_currentLine++;
	}
}

string VcpuHlInterpreter::Trim(const string& pInput)
{
	if (pInput.length() == 0)    // Empty, nothing to do
	{
		return pInput;
	}

	// Trim trailing and leading spaces
	string output = pInput;
	auto leftPosition = pInput.find_first_not_of(' ');
	auto rightPosition = pInput.find_last_not_of(' ');

	// Return untouched input string if no spaces found to trim
	if (leftPosition > pInput.length() && rightPosition > pInput.length())
	{
		return pInput;
	}

	return output.substr(leftPosition, rightPosition + 1);
}

bool VcpuHlInterpreter::ParseLine(const string& pLine)
{

}

vector<string> VcpuHlInterpreter::GetTokens(const string& pLine)
{
	vector<string> result;

	string currentToken;
	int index = 0;
	while (true)
	{
		if (index >= pLine.length())
		{
			break;
		}

		if (currentToken == "+")
		{

		}

		currentToken += pLine[index];
	}

	return result;
}
