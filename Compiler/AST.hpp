#pragma once
#include <iostream>
#include <vector>
#include "ALT.hpp"
#include "IName.hpp"
#include "SyntaxAnalyzer.hpp"

using namespace std;

class SyntaxUnit;

class AbstractSyntaxTree
{
public:
	AbstractSyntaxTree() = default;
	~AbstractSyntaxTree() = default;
//
//	void bypassLexicTree(LexicalUnit* head);
//	void consumeLexicTree(AbstractLexicTree* alt);
//	void printLexicRow(vector<LexicalUnit*>& lexicrow);
//private:
//	void determineCategory(vector<LexicalUnit*>& lexicrow);
//	void adjustTypes(vector<LexicalUnit*>& lexicrow);
//	
private:
	SyntaxUnit* head;
};


class SyntaxUnit : public IName
{
public:
	SyntaxUnit() = default;
	~SyntaxUnit() = default;

protected:
	vector<SyntaxUnit*> branch;
};

class VarDeclRef : public SyntaxUnit
{
public:
	VarDeclRef()
	{ 
		name = L"VarDeclRef";
	};

	void setVar(Variable* var);

private:
	Variable* var;
};
