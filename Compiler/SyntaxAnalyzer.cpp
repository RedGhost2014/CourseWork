#include "SyntaxAnalyzer.hpp"

void SyntaxAnalyzer::consumeLexicTree(AbstractLexicTree* alt)
{
	bypassLexicTree(alt->getHead()->getTree()[0]);
}

AbstractSyntaxTree* SyntaxAnalyzer::detachTree()
{
	AbstractSyntaxTree* currenttree = this->tree;
	this->tree = nullptr;
	return currenttree;
}

void SyntaxAnalyzer::bypassLexicTree(LexicalUnit* head)
{
	vector<LexicalUnit*> parserow;

	vector<LexicalUnit*>& currentLexicalRow = head->getTree();

	size_t i = 0;
	while (i < currentLexicalRow.size())
	{
		wstring tmp = currentLexicalRow[i]->getValue();
		parserow.push_back(currentLexicalRow[i]);
		if (tmp == L"{" || tmp == L";")
		{
			printLexicRow(parserow);
			analyze(parserow);
			parserow.clear();
		}
		i++;
	}
	if (!parserow.empty())
	{
		printLexicRow(parserow);
		analyze(parserow);
	}
}


void SyntaxAnalyzer::analyze(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow[0]->getValue() == L"}")
	{
		// TODO: Drop Scope
		lexicrow.erase(lexicrow.begin());
	}
	if (lexicrow.empty())
		return;

	if (isDeclarator(lexicrow))
	{
		/*wcout << L"Decl: ";
		for (size_t i = 0; i < lexicrow.size(); i++)
		{
			wcout << lexicrow[i]->getValue() << L" ";
		}
		wcout << endl;*/
	}


	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wstring tmp = lexicrow[i]->getValue();
		if (tmp == L"{" || tmp == L"(")
		{
			bypassLexicTree(lexicrow[i]);
		}
	}
}


vector<wstring> SyntaxAnalyzer::qualifiers = {
	L"const", L"register", L"signed", L"static", L"unsigned", L"volatile", L"nodiscard", L"immutable", L"public", L"private"
};

vector<wstring> SyntaxAnalyzer::typequalifiers = {
	L"const", L"signed", L"register", L"static", L"unsigned", L"volatile", L"public", L"private"
};

bool SyntaxAnalyzer::isBelongToAlphabet(wstring literal, vector<wstring>& alphabet)
{
	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (literal == alphabet[i])
			return true;
	}
	return false;
}

bool SyntaxAnalyzer::expectValue(LexicalUnit* target, wstring expected)
{
	return target->getValue() == expected;
}

bool SyntaxAnalyzer::expectKey(LexicalUnit* target, wstring expected)
{
	return target->getKey() == expected;
}
//
//bool SyntaxAnalyzer::isFunctionDeclarator(vector<LexicalUnit*>& lexicrow)
//{
//	if (lexicrow.size() == 0)
//		return false;
//
//	size_t index = 0;
//	while (isBelongToAlphabet(lexicrow[index]->getValue(), typequalifiers))
//	{
//		index++;
//	}
//
//	if (lexicrow[index]->getKey() == L"Keyword") // TODO:: isType
//	{
//		// TODO: MetaInfo
//		if (lexicrow[index + 1]->getKey() == L"Identifier")
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//bool SyntaxAnalyzer::isKeyword(LexicalUnit* unit)
//{
//	return unit->getKey() == L"Keyword";
//}
//
//bool SyntaxAnalyzer::isStatic(LexicalUnit* unit)
//{
//	return isKeyword(unit) && unit->getValue() == L"static";
//}
//
//bool SyntaxAnalyzer::isConst(LexicalUnit* unit)
//{
//	return isKeyword(unit) && unit->getValue() == L"const";
//}
//
//bool SyntaxAnalyzer::isQualifier(LexicalUnit* unit)
//{
//	if (isKeyword(unit))
//	{
//
//	}
//	return true;//isKeyword(unit) && unit->getValue() == L"const";
//}


void SyntaxAnalyzer::printLexicRow(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow.empty())
		return;
	static int z = 0;
	wcout << L"lexicrow[" << z << L"] = ";
	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wcout << lexicrow[i]->getValue() << " ";
	}
	wcout << endl;
	z++;
}

bool SyntaxAnalyzer::isClassDeclarator(vector<LexicalUnit*>& lexicrow)
{
	// TODO: Check Flag
	bool isFixit = true;

	wstring keyword = lexicrow[0]->getValue();
	if ((keyword == L"struct" || keyword == L"class") && lexicrow[1]->getKey() == L"Identifier")
	{
		//return true;
		if (expectValue(lexicrow[2], L";"))
		{
			return true;
		}
		// TODO: Check Flag
		else if (isFixit)
		{
			LexicalUnit* s = new LexicalUnit(lexicrow[1]->getLine());
			s->setFilename(lexicrow[1]->getFilename());
			s->setKey(L"End of line");
			s->setValue(L";");
			auto it = lexicrow.begin();
			lexicrow.insert(it + 2, s);
			Log::pushWarning(lexicrow[1]->getFilename(), L"Missed symbol: \';\' missed but emplaced by --fixit.", lexicrow[1]->getLine());

			size_t i = 0;
			while (lexicrow[i]->getValue() != L";")
			{
				lexicrow.erase(lexicrow.begin());
			}
			lexicrow.erase(lexicrow.begin());

			analyze(lexicrow);
			return true;
		}
		else
			Log::pushError(lexicrow[2]->getFilename(), L"Unexpected symbol: \';\' expected.", lexicrow[2]->getLine());
	}
	return false;
}

bool SyntaxAnalyzer::isClassDefinitor(vector<LexicalUnit*>& lexicrow)
{
	wstring keyword = lexicrow[0]->getValue();
	if ((keyword == L"struct" || keyword == L"class") && lexicrow[1]->getKey() == L"Identifier")
	{
		if (expectValue(lexicrow[2], L"{") /*|| expectValue(lexicrow[2], L";")*/)
			return true;
		else
		{
			Log::pushError(lexicrow[1]->getFilename(), L"Unresolved type declaration: \'{\' or \';\' expected.", lexicrow[1]->getLine());
		}
	}
	return false;
}

bool SyntaxAnalyzer::isDeclarator(vector<LexicalUnit*>& lexicrow)
{
	if (isClassDeclarator(lexicrow))
	{
		wcout << L"Declared new type: " << lexicrow[1]->getValue() << endl;
		return true;
	}

	if (isClassDefinitor(lexicrow))
	{
		wcout << L"Defined new type: " << lexicrow[1]->getValue() << endl;
		return true;
	}
	return false;
}