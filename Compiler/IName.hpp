#pragma once
#include <iostream>
using namespace std;

class IName
{
public:
	wstring getName();
	void setName(wstring _name);

protected:
	wstring name;
};
