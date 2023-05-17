#include "File.hpp"

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
	wcout << this->name << L"(" << m_isExist << L")";
}
