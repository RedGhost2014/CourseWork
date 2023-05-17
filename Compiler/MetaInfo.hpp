#pragma once
#include <iostream>
#include <vector>
#include "Log.hpp"
#include "ALT.hpp"
#include "IName.hpp"

using namespace std;

class Variable;

class BasicAbstractType : public IName
{
public:
	BasicAbstractType() = default;
	virtual ~BasicAbstractType() = default;

	virtual BasicAbstractType* clone() = 0;

	vector<wstring>& getSynonims();

	size_t getSize();
	void setSize(size_t _size);

	void setIsStatic(bool);
	void setIsConst(bool);
	void setIsRef(bool);
	void setIsCountOfPtrs(size_t);
	void setIsVolatile(bool);

protected:
	vector<wstring> synonyms;
	
	size_t size;

	size_t countOfPtrs;
	bool m_isConst;
	bool m_isRef;
	bool m_isStatic;
	bool m_isVolatile;
};

class PrimitiveType : public BasicAbstractType
{
public:
	PrimitiveType();
	~PrimitiveType() = default;

	PrimitiveType(const PrimitiveType& rhs);
	PrimitiveType(wstring name);
	PrimitiveType(wstring name, size_t size);

	PrimitiveType& operator=(const PrimitiveType& rhs);

	PrimitiveType* clone() override;

	void setIsUnsigned(bool);

private:

	bool m_isUnsigned;
};

class CompositeType : public BasicAbstractType 
{
public:
	CompositeType();
	CompositeType(wstring name);
	~CompositeType();


	CompositeType(const CompositeType& rhs);
	CompositeType& operator=(const CompositeType& rhs);

	bool isDefined();
	void setIsDefined(bool);

	CompositeType* clone() override;
	vector<Variable*>* getInternal();

private:
	bool m_isDefined;
	vector<Variable*>* internalTypes;

};




class Variable : public IName
{
public:
	Variable() = default;
	~Variable() = default;

	Variable(const Variable& rhs);

	BasicAbstractType* getType();
	void setType(BasicAbstractType*);

private:
	BasicAbstractType* type;
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

	vector<BasicAbstractType*>& getReturnTypes();
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
	vector<BasicAbstractType*> returnTypes;
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
	virtual void pushType(BasicAbstractType*) {};

	vector<BasicAbstractType*>& getExistedTypes();
	vector<Variable*>& getExistedVariables();
	vector<Function*>& getExistedFunctions();

protected:
	vector<BasicAbstractType*> existedTypes;
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
	void pushType(BasicAbstractType*) override;
};

class GlobalScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	GlobalScopeMetaInformation() = default;
	~GlobalScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(BasicAbstractType*) override;

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
	void pushType(BasicAbstractType*) override;
};

class TempScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	TempScopeMetaInformation() = default;
	~TempScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(BasicAbstractType*) override;
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
	BasicAbstractType* getTypeByName(wstring name);
	Function* getFunctionByName(wstring name);

	vector<AbstractScopeMetaInformation*>& getMetaStack();
private:
	vector<AbstractScopeMetaInformation*> metaStack;
};

extern MetaInformaton MetaInfo;