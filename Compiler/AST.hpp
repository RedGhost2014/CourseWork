#pragma once
#include <iostream>
#include <vector>
#include "ALT.hpp"
#include "IName.hpp"
#include "SyntaxAnalyzer.hpp"

using namespace std;


class ISignature
{
public:
	wstring getSignature();
	void setSignature(wstring _signature);

protected:
	wstring signature;
};

class SyntaxUnit : public IName, public ISignature
{
public:
	SyntaxUnit() = default;
	virtual ~SyntaxUnit() = default;

	vector<SyntaxUnit*>& getBranch();
	void push(SyntaxUnit*);

	virtual void print() { wcout << name; };

protected:
	vector<SyntaxUnit*> branch;
};

class AbstractSyntaxTree
{
public:
	AbstractSyntaxTree();
	~AbstractSyntaxTree() = default;

	SyntaxUnit& getHead();
	void push(SyntaxUnit*);
	void pop();

	void print();

private:
	void treePrint(SyntaxUnit*);

	SyntaxUnit head;
	vector<vector<SyntaxUnit*>*> stack;
};

class TranslationUnit : public SyntaxUnit
{
public:
	TranslationUnit() { name = L"TranslationUnit"; }
	void print() override;
};

class CompoundStatement : public SyntaxUnit
{
public:
	CompoundStatement(size_t _stringnumber) : stringnumber(_stringnumber) { name = L"CompoundStatement"; }
	void print() override;
private:
	size_t stringnumber;
};

class ReturnStatement : public SyntaxUnit
{
public:
	ReturnStatement(size_t _stringnumber) : stringnumber(_stringnumber) { name = L"ReturnStatement"; }
	void print() override;
private:
	size_t stringnumber;
};


class OperatorReference : public SyntaxUnit
{
public:
	OperatorReference() { name = L"OperatorRef"; }

	void print() override;

	void setOperator(BasicAbstractOperator* op);
	BasicAbstractOperator* getOperator();
private:
	BasicAbstractOperator* op;
};

class VariableDeclReference : public SyntaxUnit
{
public:
	VariableDeclReference() { name = L"VarRef"; };
	void print() override;

	void setVariable(Variable* var);
	Variable* getVariable();

private:
	Variable* var;
};


class Register;
// Used to determine temporary values after calculation via rpn
class TempValueReference : public SyntaxUnit
{
	Register* usedreg;
public:
	TempValueReference() = default;
	TempValueReference(Register* reg)
	{
		usedreg = reg;
		name = L"TempValueReference";
	};
	Register* getRegister() { return usedreg; };
};

class FunctionDeclReference : public SyntaxUnit
{
public:
	FunctionDeclReference()
	{
		name = L"FunDeclRef";
	};

	void setFunction(Function* f);
	Function* getFunction();
	void print() override;

private:
	Function* function;
};

class FunctionCallReference : public SyntaxUnit
{
public:
	FunctionCallReference()
	{
		name = L"FunCallRef";
	};

	void setFunction(Function* f);
	Function* getFunction();

	void print() override;

private:
	Function* function;
};


class ConstantDeclReference : public SyntaxUnit
{
public:
	ConstantDeclReference()
	{
		name = L"ConstantRef";
	};
	void print() override;

	void setType(BasicAbstractType* t);
	BasicAbstractType* getType();
private:
	BasicAbstractType* constantType;
};

