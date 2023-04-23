#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Type;
class Variable;

class Type
{
public:
	Type() = default;
	Type(wstring name, size_t size);
	Type(const Type&) = default;
	~Type() = default;

	void setName(wstring _name);
	void setSize(size_t _size);
	wstring getName();
	virtual size_t getSize();

private:

	vector<Variable*>* internalTypes;
	wstring name;
	size_t size;

	size_t countOfPtrs;
	bool m_isConst;
	bool m_isVolatile;
	bool m_isUnsigned;
};

class Variable
{
public:
	Variable() = default;
	~Variable() = default;

	wstring getIdentifier();
	Type& getType();

private:
	Type type;
	wstring identifier;
};


class Operator
{
public:
	Operator() = default;
	~Operator() = default;

private:

};


class Function
{
public:
	Function() = default;
	~Function() = default;

private:
	
};

class ScopeMetaInformation
{
public:
	ScopeMetaInformation();
	ScopeMetaInformation(const ScopeMetaInformation&) = default;
	ScopeMetaInformation(ScopeMetaInformation&&) = default;
	~ScopeMetaInformation() = default;
	
	void pushDeclaredType(Type);
	void pushDefinedType(Type);

	void printDeclaredTypes();
	void printDefinedTypes();
private:
	vector<Type> declaredTypes;
	vector<Type> definedTypes;
	vector<Variable> existedVariables;
	vector<Function> existedFunctions;
};

class ScopeMetaInformation;

class MetaInformaton
{
public:
	MetaInformaton();
	~MetaInformaton();
	
	void isTypeExist();

	vector<ScopeMetaInformation>& getMetaStack();
private:
	vector<ScopeMetaInformation> metaStack;
};