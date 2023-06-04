#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Preprocessor
{
public:
	Preprocessor() = default;
	~Preprocessor() = default;

	Preprocessor& open(wstring filename);
	Preprocessor& close();
	Preprocessor& parse();
	wstring getPreprocessedFilename();

private:

	wstring retriveBrackets(wstring& base);
	wstring dropBrackets(wstring& base);

	wstring erase(wstring& base, wstring symbols);

	bool containBrackets(wstring& base);

	void replaceDefines(wstring& str);
	wstring preprocessedName;
	wstring originalName;

	vector<pair<vector<wstring>, wstring>> functiondefs;
	vector<pair<wstring, wstring>> basicdefs;
	vector<wstring> macroses;

	wifstream input;
	wofstream output;
};
