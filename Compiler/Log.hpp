#pragma once
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class LexicalUnit;

class Log
{
public:
	static size_t getErrorsCount();
	static size_t getWarningsCount();

	static void pushError(wstring error, LexicalUnit* unit);
	static void pushError(wstring _file, wstring error, size_t _line);
	static void pushWarning(wstring warning, LexicalUnit* unit);
	static void pushWarning(wstring _file, wstring warning, size_t _line);
	static void print(wstring);
};

class ErrorMessage
{
public:

	static wstring syntaxError;

	static wstring bracket_Open_Round_Missed;
	static wstring bracket_Close_Round_Missed;
	static wstring bracket_Open_Figure_Missed;
	static wstring bracket_Close_Figure_Missed;
	static wstring bracket_Open_Square_Missed;
	static wstring bracket_Close_Square_Missed;
};
