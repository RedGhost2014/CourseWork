#include "MetaInfo.hpp"

Type::Type(wstring _name, size_t _size)
{
	name = _name;
	size = _size;
}

void Type::setName(wstring _name)
{
	name = _name;
}

void Type::setSize(size_t _size)
{
	size = _size;
}

wstring Type::getName() { return name; }
size_t Type::getSize() { return size; }


//vector<CompositeType>& CompositeType::getInternalTypes()
//{
//	return internalTypes;
//}
//
//size_t CompositeType::getSize()
//{
//	size_t result = 0;
//	for (size_t i = 0; i < internalTypes.size(); i++)
//	{
//		result += internalTypes[i].getSize();
//	}
//}






wstring Variable::getIdentifier()
{
	return identifier;
}
Type& Variable::getType()
{
	return type;
}











ScopeMetaInformation::ScopeMetaInformation()
{

}

void ScopeMetaInformation::pushDefinedType(Type t)
{
	definedTypes.push_back(t);
}

void ScopeMetaInformation::printDefinedTypes()
{
	wcout << L"Defined Types:" << endl;
	for (size_t i = 0; i < definedTypes.size(); i++)
	{
		wcout << L"Type[" << i << L"] = (" << definedTypes[i].getName() << ", " << definedTypes[i].getSize() << L")" << endl;
	}
}

void ScopeMetaInformation::printDeclaredTypes()
{
	wcout << L"Declared Types: " << declaredTypes.size() << endl;
	for (size_t i = 0; i < declaredTypes.size(); i++)
	{
		wcout << L"Type[" << i << L"] = (" << declaredTypes[i].getName() << ", " << declaredTypes[i].getSize() << L")" << endl;
	}
}

MetaInformaton::MetaInformaton()
{
	ScopeMetaInformation toplevel;
	toplevel.pushDefinedType(Type(L"char", 1));
	toplevel.pushDefinedType(Type(L"bool", 1));
	toplevel.pushDefinedType(Type(L"short", 2));
	toplevel.pushDefinedType(Type(L"int", 4));
	toplevel.pushDefinedType(Type(L"long long", 8));
	metaStack.push_back(toplevel);
}

vector<ScopeMetaInformation>& MetaInformaton::getMetaStack()
{
	return metaStack;
}