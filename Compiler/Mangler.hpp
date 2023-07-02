#pragma once
#include <iostream>
#include "MetaInfo.hpp"
#include "Utils.hpp"

class BasicMangler
{
public:
	BasicMangler() = default;
	~BasicMangler() = default;

	virtual wstring mangleVariableName(wstring var);
	virtual wstring mangleFunction(Function* f);

};

class Mangler : public BasicMangler
{
public:
	Mangler() = default;
	~Mangler() = default;

	wstring mangleVariableName(wstring var) override;
	wstring mangleFunction(Function* f) override;
};