#pragma once
#include <iostream>
#include <vector>
#include "ALT.hpp"
#include "IName.hpp"
#include "SyntaxAnalyzer.hpp"

using namespace std;


class SyntaxUnit : public IName
{
public:
	SyntaxUnit() = default;
	virtual ~SyntaxUnit() = default;

	vector<SyntaxUnit*>& getBranch();
	void push(SyntaxUnit*);

protected:
	vector<SyntaxUnit*> branch;
};

class AbstractSyntaxTree
{
public:
	AbstractSyntaxTree() = default;
	~AbstractSyntaxTree() = default;

	SyntaxUnit& getHead();
	void push(SyntaxUnit*);
//
//	void bypassLexicTree(LexicalUnit* head);
//	void consumeLexicTree(AbstractLexicTree* alt);
//	void printLexicRow(vector<LexicalUnit*>& lexicrow);
//private:
//	void determineCategory(vector<LexicalUnit*>& lexicrow);
//	void adjustTypes(vector<LexicalUnit*>& lexicrow);
//	
private:
	SyntaxUnit head;
	vector<vector<SyntaxUnit*>*> stack;
};

class TranslationUnit : public SyntaxUnit
{
public:
	TranslationUnit() { name = L"TranslationUnit"; }
};

class CompoundStatement : public SyntaxUnit
{
public:
	CompoundStatement() { name = L"CompoundStatement"; }
};

class OperatorReference : public SyntaxUnit
{
public:
	OperatorReference() { name = L"OperatorReference"; }
	void setOperatorValue(Operator* op);
private:
	Operator* op;
};

class VariableDeclReference : public SyntaxUnit
{
public:
	VariableDeclReference()
	{ 
		name = L"VarRef";
	};

	void setVariable(Variable* var);

private:
	Variable* var;
};

class FunctionDeclReference : public SyntaxUnit
{
public:
	FunctionDeclReference()
	{
		name = L"FunDeclRef";
	};

	void setFunction(Function* f);

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

	void setConstant(wstring constant);
	void setType(BasicAbstractType* t);
private:
	wstring constant;
	BasicAbstractType* constantType;
};