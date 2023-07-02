#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

class Utils
{
public:
	static std::wstring exePath();
	static std::wstring exeName();
	static bool isFileExists(const std::wstring& str);
	static wstring getFileExtension(const std::wstring& str);
	static bool isExistFileExtension(const std::wstring& str);
	static wstring toWstring(size_t number, int radix);
};