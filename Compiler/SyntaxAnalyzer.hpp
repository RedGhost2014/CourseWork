#include "ALT.hpp"
#include "AST.hpp"
#include "Log.hpp"


class AbstractSyntaxTree;
class AbstractLexicTree;

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

	bool isBelongToAlphabet(wstring literal, vector<wstring>& alphabet);
	bool expectValue(LexicalUnit* target, wstring expected);
	bool expectKey(LexicalUnit* target, wstring expected);

	static vector<wstring> qualifiers;
	static vector<wstring> typequalifiers;


	bool isDeclarator(vector<LexicalUnit*>& lexicrow);
	bool isFunctionDeclarator(vector<LexicalUnit*>& lexicrow);
	bool isClassDeclarator(vector<LexicalUnit*>& lexicrow);
	bool isClassDefinitor(vector<LexicalUnit*>& lexicrow);

	bool isTypeDefinition(vector<LexicalUnit*>& lexicrow);
	bool isKeyword(LexicalUnit*);

	bool isQualifier(LexicalUnit*);
	bool isStatic(LexicalUnit*);
	bool isConst(LexicalUnit*);

	AbstractSyntaxTree* tree;
};
