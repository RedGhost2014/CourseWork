#include "MetaInfo.hpp"

MetaInformaton MetaInfo;


Type::Type(wstring name)
{
	internalTypes = nullptr;
	size = 1;
	this->name = name;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	countOfPtrs = 0;
}

Type::Type(wstring name)
{
	internalTypes = nullptr;
	size = 1;
	this->name = name;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	countOfPtrs = 0;
}

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





void Variable::setIdentifier(wstring _identifier)
{
	identifier = _identifier;
}
wstring Variable::getIdentifier()
{
	return identifier;
}
Type& Variable::getType()
{
	return type;
}











AbstractScopeMetaInformation::AbstractScopeMetaInformation() {}

vector<Type*>& AbstractScopeMetaInformation::getExistedTypes() { return existedTypes; };
vector<Variable*>& AbstractScopeMetaInformation::getExistedVariables() { return existedVariables;  };
vector<Function*>& AbstractScopeMetaInformation::getExistedFunctions() { return existedFunctions; };

//void ScopeMetaInformation::printDefinedTypes()
//{
//	wcout << L"Defined Types:" << endl;
//	for (size_t i = 0; i < definedTypes.size(); i++)
//	{
//		wcout << L"Type[" << i << L"] = (" << definedTypes[i].getName() << ", " << definedTypes[i].getSize() << L")" << endl;
//	}
//}
//
//void ScopeMetaInformation::printDeclaredTypes()
//{
//	wcout << L"Declared Types: " << declaredTypes.size() << endl;
//	for (size_t i = 0; i < declaredTypes.size(); i++)
//	{
//		wcout << L"Type[" << i << L"] = (" << declaredTypes[i].getName() << ", " << declaredTypes[i].getSize() << L")" << endl;
//	}
//}

//
//bool ScopeMetaInformation::isTypeExist(wstring _typename)
//{
//	for (Type& v : declaredTypes)
//	{
//		if (v.getName() == _typename)
//			return true;
//	}
//
//	for (Type& v : definedTypes)
//	{
//		if (v.getName() == _typename)
//			return true;
//	}
//
//	return false;
//}
//
//
//bool MetaInformaton::isTypeExist(wstring _typename)
//{
//	for (ScopeMetaInformation& v : metaStack)
//	{
//		if (v.isTypeExist(_typename))
//		{
//			return true;
//		}
//	}
//	return false;
//}

void GlobalScopeMetaInformation::pushFunction(Function* f)
{
	existedFunctions.push_back(f);
}

void GlobalScopeMetaInformation::pushType(Type* t)
{
	existedTypes.push_back(t);
}

void GlobalScopeMetaInformation::pushVariable(Variable* v)
{
	existedVariables.push_back(v);
}



















void MetaInformaton::pushScope(AbstractScopeMetaInformation scope)
{
	metaStack.push_back(scope);
}

Type* MetaInformaton::getTypeByName(wstring name)
{
	Type* result = nullptr;

	for (AbstractScopeMetaInformation& meta : metaStack)
	{
		vector<Type*>& types = meta.getExistedTypes();
		for (size_t i = 0; i < types.size(); i++)
		{
			if (types[i]->getName() == name)
			{
				result = types[i];
				return result;
			}
		}
	}
	return result;
}

MetaInformaton::MetaInformaton()
{
	GlobalScopeMetaInformation toplevel;
	toplevel.pushType(new Type(L"char", 1));
	toplevel.pushType(new Type(L"bool", 1));
	toplevel.pushType(new Type(L"short", 2));
	toplevel.pushType(new Type(L"int", 4));
	toplevel.pushType(new Type(L"float", 4));
	toplevel.pushType(new Type(L"double", 4));
	toplevel.pushType(new Type(L"long", 8));

	this->pushScope(toplevel);
}

vector<AbstractScopeMetaInformation>& MetaInformaton::getMetaStack()
{
	return metaStack;
}