#pragma once
#include <iostream>
#include <vector>
#include "Log.hpp"
#include "ALT.hpp"
#include "IName.hpp"

using namespace std;

class Type;
class Variable;


class Type : public IName
{
public:
	Type();
	Type(wstring name);
	Type(wstring name, size_t size);
	Type(wstring name, size_t size, bool);


	Type(const Type&);
	Type& operator=(const Type&);


	~Type() = default;

	void setSize(size_t _size);

	Type& setIsConst(bool);
	Type& setIsUnsigned(bool);
	Type& setIsVolatile(bool);
	Type& setIsRef(bool);
	Type& setIsDefined(bool);
	Type& setIsCountOfPtrs(size_t);

	bool isDefined();

	size_t getSize();
	vector<wstring>& getSynonims();
	vector<Variable*>* getInternal();

private:

	vector<Variable*>* internalTypes;
	vector<wstring> synonyms;
	size_t size;

	size_t countOfPtrs;
	bool m_isConst;
	bool m_isRef;
	bool m_isVolatile;
	bool m_isUnsigned;

	bool m_isDefined;
};

class Variable : public IName
{
public:
	Variable() = default;
	~Variable() = default;

	Type& getType();
	void setType(Type&);

private:
	Type type;
};


class Operator
{
public:
	Operator() = default;
	~Operator() = default;

private:

};


class Function : public IName
{
public:
	Function();
	~Function() = default;

	vector<Type*>& getReturnTypes();
	vector<Variable*>& getFunctionArguments();
	vector<Function*>& getFunctionOverloads();

	bool isDefined();
	void setDefinitionStatus(bool);
	size_t getStackSize();
	void setStackSize(size_t newsize);

private:
	bool m_isDefined;
	size_t stackSize;
	vector<Function*> functionOverloads;
	vector<Type*> returnTypes;
	vector<Variable*> functionArguments;
};


class AbstractScopeMetaInformation
{
public:
	AbstractScopeMetaInformation();
	AbstractScopeMetaInformation(const AbstractScopeMetaInformation&) = default;
	AbstractScopeMetaInformation(AbstractScopeMetaInformation&&) = default;
	virtual ~AbstractScopeMetaInformation() = default;

	// IPush
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
	LexicalUnit* start;
public:
	ClassScopeMetaInformation() = default;
	ClassScopeMetaInformation(LexicalUnit*);
	~ClassScopeMetaInformation() override;

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

class FunctionScopeMetaInformation : public AbstractScopeMetaInformation
{
	Function* basicFunction;
	LexicalUnit* start;
public:
	FunctionScopeMetaInformation() = default;
	FunctionScopeMetaInformation(Function* currentFunction, LexicalUnit* _start);
	~FunctionScopeMetaInformation() override;

	Function* getBasicFunction();

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(Type*) override;
};

class TempScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	TempScopeMetaInformation() = default;
	~TempScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(Type*) override;
};


class MetaInformaton
{
public:
	MetaInformaton();
	~MetaInformaton() = default;

	void pushScope(AbstractScopeMetaInformation*);
	void popScope();
	AbstractScopeMetaInformation* back();

	Variable* getVariableByName(wstring name);
	Type* getTypeByName(wstring name);
	Function* getFunctionByName(wstring name);

	vector<AbstractScopeMetaInformation*>& getMetaStack();
private:
	vector<AbstractScopeMetaInformation*> metaStack;
};

extern MetaInformaton MetaInfo;