#pragma once
#include <iostream>
#include <vector>
using namespace std;

class File
{
public:
	File() = default;
	~File() = default;

	File& setName(wstring name);
	wstring getName();

	File& setExistState(bool);
	bool isExist();

	void dbgprint();

private:
	bool m_isExist;
	wstring m_name;
	wstring m_path;
};
