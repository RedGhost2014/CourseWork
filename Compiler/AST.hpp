#pragma once
#include <iostream>
#include <vector>
#include "ALT.hpp"

using namespace std;

class SyntaxUnit;

class AbstractSyntaxTree
{
public:
	AbstractSyntaxTree() = default;
	~AbstractSyntaxTree() = default;

	void bypassLexicTree(LexicalUnit* head);
	void consumeLexicTree(AbstractLexicTree* alt);
	void printLexicRow(vector<LexicalUnit*>& lexicrow);
private:
	void determineCategory(vector<LexicalUnit*>& lexicrow);
	void adjustTypes(vector<LexicalUnit*>& lexicrow);

	SyntaxUnit* head;
};


class SyntaxUnit
{
public:
	SyntaxUnit() = default;
	~SyntaxUnit() = default;

protected:
	wstring type;
	vector<SyntaxUnit*> branch;
};

class Declarator : SyntaxUnit
{
public:
	Declarator() { type = L"Declarator"; };

private:

};
