#include "File.hpp"


File& File::setName(wstring name)
{
	m_name = name;
	return *this;
}

wstring File::getName()
{
	return m_name;
}

File& File::setExistState(bool state)
{
	m_isExist = state;
	return *this;
}

bool File::isExist()
{
	return m_isExist;
}

void File::dbgprint()
{
	wcout << this->m_name << L"(" << m_isExist << L")";
}
