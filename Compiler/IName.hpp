#pragma once
#include <iostream>
using namespace std;

// P.S Hi crabalbert :)
class IName
{
public:
	wstring getName() const;
	void setName(wstring _name);

protected:
	wstring name;
};
