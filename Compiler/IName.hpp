#pragma once
#include <iostream>
using namespace std;

// ѕрекрасно знаю что это не интерфейс а абстрактный класс,
// но дл€ того чтобы не было путаницы между пон€ти€ми €зыкового анализа, и чтобы побесить одного человека, во всем проекте эти два пон€ти€ противоположны
// P.S ѕривет јльберто :)
class IName
{
public:
	wstring getName();
	void setName(wstring _name);

protected:
	wstring name;
};
