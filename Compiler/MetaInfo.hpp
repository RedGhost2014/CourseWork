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
	size_t getCountOfPointers();
	bool isStatic();
	bool isConst();
	bool isRef();
	bool isVolatile();

	void setSize(size_t _size);

	void setIsStatic(bool);
	void setIsConst(bool);
	void setIsRef(bool);
	void setCountOfPtrs(size_t);
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
	vector<Variable*>*& getInternal();

private:
	bool m_isDefined;
	vector<Variable*>* internalTypes;

};




class Variable : public IName
{
public:
	Variable() = default;
	~Variable() {};

	Variable(const Variable& rhs);

	BasicAbstractType* getType();
	void setType(BasicAbstractType*);

private:
	BasicAbstractType* type;
};


class BasicAbstractOperator : public IName
{
public:
	virtual ~BasicAbstractOperator() = default;
	size_t getPrecedence();
	bool isCanBeOverloaded();
	bool isBuiltIn();
	int getAssociativity();
	void setAssociativity(int);

protected:
	size_t precedence;
	bool canBeOverloaded;
	bool _isBuiltIn;

	// 1 - left-to-right
	// -1 - right-to-left
	int associativity;
};

class UnaryOperator : public BasicAbstractOperator
{
public:
	UnaryOperator(wstring op, size_t _precedence, int _associativity, bool canBeOverloaded, bool isBuiltIn);
	~UnaryOperator() = default;

	BasicAbstractType* getType();
	void setType(BasicAbstractType* t);

private:
	BasicAbstractType* type;
};

class BinaryOperator : public BasicAbstractOperator
{
public:
	BinaryOperator(wstring op, size_t _precedence, int _associativity, bool canBeOverloaded, bool isBuiltIn);
	~BinaryOperator() = default;

	BasicAbstractType* getLeftType();
	BasicAbstractType* getRightType();

	void setLeftType(BasicAbstractType* t);
	void setRightType(BasicAbstractType* t);

private:
	BasicAbstractType* leftType;
	BasicAbstractType* rightType;

};

class Function : public IName
{
public:
	Function();
	~Function() = default;

	vector<BasicAbstractType*>& getReturnTypes();
	vector<Variable*>& getFunctionArguments();
	vector<Variable*>& getFunctionStackVars();
	vector<Function*>& getFunctionOverloads();

	bool isDefined();
	void setDefinitionStatus(bool);
	size_t getStackSize();
	void setStackSize(size_t newsize);
	void setStackVariables(vector<Variable*>& _stackvars);

private:
	bool m_isDefined;
	size_t stackSize;

	vector<Variable*> functionStackVariables;

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

	virtual void pushFunction(Function*) {};
	virtual void pushVariable(Variable*) {};
	virtual void pushType(BasicAbstractType*) {};
	virtual void pushOperator(BasicAbstractOperator*) {};

	vector<BasicAbstractType*>& getExistedTypes();
	vector<Variable*>& getExistedVariables();
	vector<Function*>& getExistedFunctions();
	vector<BasicAbstractOperator*>& getExistedOperators();

protected:
	vector<BasicAbstractType*> existedTypes;
	vector<Variable*> existedVariables;
	vector<Function*> existedFunctions;
	vector<BasicAbstractOperator*> existedOperators;
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
	void pushOperator(BasicAbstractOperator*) override;
};

class GlobalScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	GlobalScopeMetaInformation() = default;
	~GlobalScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(BasicAbstractType*) override;
	void pushOperator(BasicAbstractOperator*) override;

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
	void pushOperator(BasicAbstractOperator*) override;
};

class TempScopeMetaInformation : public AbstractScopeMetaInformation
{
public:
	TempScopeMetaInformation() = default;
	~TempScopeMetaInformation() = default;

	void pushFunction(Function*) override;
	void pushVariable(Variable*) override;
	void pushType(BasicAbstractType*) override;
	void pushOperator(BasicAbstractOperator*) override;
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

	BasicAbstractOperator* getBinaryOperatorByName(wstring name);
	BasicAbstractOperator* getUnaryOperatorByName(wstring name);
	BasicAbstractOperator* getUnaryOperatorByNameWithAssociativity(wstring name,  int _accos);

	vector<AbstractScopeMetaInformation*>& getMetaStack();

	bool isPureIntegralType(BasicAbstractType* t);
	bool isPrimitiveType(BasicAbstractType* t);

private:
	vector<AbstractScopeMetaInformation*> metaStack;
};

extern MetaInformaton MetaInfo;