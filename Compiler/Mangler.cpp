#include "Mangler.hpp"

wstring BasicMangler::mangleFunction(Function* f)
{
	return f->getName();
}

wstring BasicMangler::mangleVariableName(wstring var)
{
	return L"_" + var + L"$";
	//return var;
}

wstring Mangler::mangleFunction(Function* f)
{
	hash<wstring> h;

	return L"_" + f->getName() + L"_" + Utils::toWstring(h(f->getName()), 16);
}

wstring Mangler::mangleVariableName(wstring var)
{
	return L"_" + var + L"$";
}