#pragma once
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Log
{
public:
	static size_t getErrorsCount();
	static size_t getWarningsCount();

	static void pushError(wstring _file, wstring error, size_t _line);
	static void pushWarning(wstring _file, wstring warning, size_t _line);
	static void print(wstring);
};

