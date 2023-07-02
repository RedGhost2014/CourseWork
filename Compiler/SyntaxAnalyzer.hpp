#pragma once
#include "Log.hpp"
#include "MetaInfo.hpp"
#include "ALT.hpp"

class AbstractSyntaxTree;

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer();

	~SyntaxAnalyzer() = default;

	void addSourceFileToAST(File* files);

	AbstractSyntaxTree* detachTree();
	AbstractSyntaxTree* getTree();
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

	bool isReturnStatement(vector<LexicalUnit*>& lexicrow);
	
	// Declarators
	bool isDeclarator(vector<LexicalUnit*>& lexicrow);
	
	bool isClassDeclarator(vector<LexicalUnit*>& lexicrow);

	bool isTypedefDeclarator(vector<LexicalUnit*>& lexicrow);

	bool isVariableDeclarator(vector<LexicalUnit*>& lexicrow);
	
	bool isFunctionDeclarator(vector<LexicalUnit*>& lexicrow);
	void parseFunctionParentheses(vector<LexicalUnit*>& internalLexicRow, vector<Variable*>& functionArgumentsToFill);

	BasicAbstractType* retrieveType(vector<LexicalUnit*>& lexicrow, size_t* countOfParsedUnits);


	AbstractSyntaxTree* tree;
};
