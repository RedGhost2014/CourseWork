#pragma once
#include <iostream>
#include "File.hpp"

using namespace std;

class Settings
{
public:
	Settings() = default;
	~Settings() = default;
	Settings& parse(wchar_t* argv[], size_t argc);
	//void readProjectInfo(string name);
	void dbgprint();
	vector<File*> getSourceFiles();

	bool isContainFlag(wstring);

private:

	void filter(wchar_t* argv[], size_t argc);

	vector<File*> sourceFiles;

	vector<wstring> flags;
	vector<wstring> names;

};
