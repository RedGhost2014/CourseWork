#pragma once
#include <iostream>
#include <vector>
#include "IName.hpp"

using namespace std;

class File : public IName
{
public:
	File() = default;
	~File() = default;

	File& setExistState(bool);
	bool isExist();

	void dbgprint();

private:
	bool m_isExist;
	wstring m_path;
};
