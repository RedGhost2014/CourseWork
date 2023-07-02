#include "SyntaxAnalyzer.hpp"
#include "AST.hpp"

class ReversePolishNotationCreator
{
public:
	ReversePolishNotationCreator() = default;
	~ReversePolishNotationCreator() = default;

	// "It just works"
	void createRPN(vector<LexicalUnit*>& sequence, AbstractSyntaxTree* treeToFill);

private:

	void resolveLexicSequenceIntoSyntax(vector<LexicalUnit*>& sequence, vector<SyntaxUnit*>& result);
	void resolveBinaryOperatorSignature(wstring operatorSignatureSequence, vector<SyntaxUnit*>& result);
	void resolveUnaryOperatorSignature(wstring operatorSignatureSequence, vector<SyntaxUnit*>& result);

	// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
	void determineSyntaxUnit(SyntaxUnit* currentUnit, vector<SyntaxUnit*>& stack, vector<SyntaxUnit*>& output);
};
