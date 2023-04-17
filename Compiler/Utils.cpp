#include "Utils.hpp"

std::wstring Utils::exePath()
{
	WCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

std::wstring Utils::exeName()
{
	WCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(pos);
}

bool Utils::isFileExists(const std::wstring& str)
{
	wifstream file(str);
	return file.is_open();
}

wstring Utils::getFileExtension(const std::wstring& str)
{
	wstring s = str;
	size_t shift = s.find_last_of('\\.');
	return s.substr(shift);
	//return shift != s.npos;

}

bool Utils::isExistFileExtension(const std::wstring& str)
{
	wstring s = str;
	size_t shift = s.find_last_of('\\.');
	//return s.substr(shift);
	return shift != s.npos;

}