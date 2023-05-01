#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Type;
class Variable;

class Type
{
public:
	Type(wstring name);
	Type(wstring name, size_t size);
	Type(const Type&) = default;
	~Type() = default;

	void setName(wstring _name);
	void setSize(size_t _size);
	wstring getName();
	size_t getSize();

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
	void setIdentifier(wstring identifier);
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


class AbstractScopeMetaInformation
{
public:
	AbstractScopeMetaInformation();
	AbstractScopeMetaInformation(const AbstractScopeMetaInformation&) = default;
	AbstractScopeMetaInformation(AbstractScopeMetaInformation&&) = default;
	~AbstractScopeMetaInformation() = default;
	
	// IPushable
	virtual void pushFunction(Function*) {};
	virtual void pushVariable(Variable*) {};
	virtual void pushType(Type*) {};

	vector<Type*>& getExistedTypes();
	vector<Variable*>& getExistedVariables();
	vector<Function*>& getExistedFunctions();

protected:
	vector<Type*> existedTypes;
	vector<Variable*> existedVariables;
	vector<Function*> existedFunctions;
};

class ClassScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	ClassScopeMetaInformation() = default;
	~ClassScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(Type*) override;
};


class GlobalScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	GlobalScopeMetaInformation() = default;
	~GlobalScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(Type*) override;

};

class MetaInformaton
{
public:
	MetaInformaton();
	~MetaInformaton() = default;

	void pushScope(AbstractScopeMetaInformation);

	Type* getTypeByName(wstring name);

	vector<AbstractScopeMetaInformation>& getMetaStack();
private:
	vector<AbstractScopeMetaInformation> metaStack;
};

extern MetaInformaton MetaInfo;