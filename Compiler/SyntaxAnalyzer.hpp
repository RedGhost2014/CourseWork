#pragma once
#include "AST.hpp"
#include "Log.hpp"
#include "MetaInfo.hpp"
#include "ALT.hpp"

//class AbstractSyntaxTree;
//class AbstractLexicTree;

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer() = default;
	~SyntaxAnalyzer() = default;

	AbstractSyntaxTree* detachTree();
	void consumeLexicTree(AbstractLexicTree* alt);

private:
	// Debug functions
	void printLexicRow(vector<LexicalUnit*>& lexicrow);

private:
	void bypassLexicTree(LexicalUnit* head);

	void analyze(vector<LexicalUnit*>&);
	void leftDropTillSemi(vector<LexicalUnit*>&);


	bool isBelongToAlphabet(wstring literal, vector<wstring>& alphabet);
	bool expectValue(LexicalUnit* target, wstring expected);
	bool expectKey(LexicalUnit* target, wstring expected);

	static vector<wstring> qualifiers;
	static vector<wstring> typequalifiers;

	bool isDefinition(vector<LexicalUnit*>& lexicrow);
	// Declarator
	bool isDeclarator(vector<LexicalUnit*>& lexicrow);
	bool isClassDeclarator(vector<LexicalUnit*>& lexicrow);

	bool isVariableDeclarator(vector<LexicalUnit*>& lexicrow);



	bool isFunctionDeclarator(vector<LexicalUnit*>& lexicrow);


	bool isTypeDefinition(vector<LexicalUnit*>& lexicrow);
	bool isKeyword(LexicalUnit*);

	bool isQualifier(LexicalUnit*);
	bool isStatic(LexicalUnit*);
	bool isConst(LexicalUnit*);

	AbstractSyntaxTree* tree;
};
