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
			//printLexicRow(parserow);
			
			while (!parserow.empty())
			{
				analyze(parserow);
				leftDropTillSemi(parserow);
			}
		}
		i++;
	}
	if (!parserow.empty())
	{
		//printLexicRow(parserow);
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
	if (lexicrow.empty() || expectValue(lexicrow[0], L";"))
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
	else
	{

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

void SyntaxAnalyzer::leftDropTillSemi(vector<LexicalUnit*>& lexicrow)
{
	size_t i = 0;
	while (!lexicrow.empty() && lexicrow[i]->getValue() != L";")
	{
		lexicrow.erase(lexicrow.begin());
	}
	if (!lexicrow.empty())
		lexicrow.erase(lexicrow.begin());
}

bool SyntaxAnalyzer::isDefinition(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow.empty())
		return false;
	return lexicrow.back()->getValue() == L"{";
}

bool SyntaxAnalyzer::isClassDeclarator(vector<LexicalUnit*>& lexicrow)
{
	// TODO: Check Flag
	static bool isFixit = false;

	wstring keyword = lexicrow[0]->getValue();
	if ((keyword == L"struct" || keyword == L"class"))
	{
		if (lexicrow[1]->getKey() != L"Identifier")
		{
			Log::pushError(ErrorMessage::syntaxError + L"Identifier expected.", lexicrow[1]);
			return false;
		}
		if (expectValue(lexicrow[2], L";"))
		{
			wstring name = lexicrow[1]->getValue();
			if (!MetaInfo.getTypeByName(name))
				MetaInfo.getMetaStack().back().pushType(new Type(name));
			return true;
		}
		else if (expectValue(lexicrow[2], L"{"))
		{
			wstring name = lexicrow[1]->getValue();
			if (!MetaInfo.getTypeByName(name))
				MetaInfo.getMetaStack().back().pushType(new Type(name));

			ClassScopeMetaInformation scope;
			MetaInfo.pushScope(scope);
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
			
			Log::pushWarning(ErrorMessage::syntaxError + L"\';\' missed but emplaced by --fixit.", lexicrow[1]);

			wstring name = lexicrow[1]->getValue();
			if (!MetaInfo.getTypeByName(name))
				MetaInfo.getMetaStack().back().pushType(new Type(name));
			return true;
		}
		else Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'{\' expected.", lexicrow[1]);
	}
	return false;
}

bool SyntaxAnalyzer::isVariableDeclarator(vector<LexicalUnit*>& lexicrow)
{
	bool isStatic = false;
	bool isRef = false;

	//bool is = false;
	size_t countOfUnsigned = 0;
	size_t countOfPtrs = 0;
	size_t countOfConst = 0;
	wstring typeName;


	size_t i = 0;

	while (i < lexicrow.size() && !MetaInfo.getTypeByName(lexicrow[i]->getValue()))
	{
		wstring key = lexicrow[i]->getKey();
		wstring value = lexicrow[i]->getValue();
		if (key == L"Keyword")
		{
			if (value == L"static")
			{
				if (!isStatic)
					isStatic = true;
				else
				{
					Log::pushError(L"Storage class can be declared only once.", lexicrow[i]);
				}
			}
			else if (value == L"const")
			{
				countOfConst++;
			}
			else if (value == L"unsigned")
			{
				countOfUnsigned++;
			}
		}
		i++;
	}
	typeName = lexicrow[i]->getValue();

	if (countOfConst > 1)
	{
		Log::pushWarning(L"Qualififer \"const\" repeated more that 1 time.", lexicrow[0]);
	}
	if (countOfUnsigned > 1)
	{
		Log::pushWarning(L"Qualififer \"unsigned\" repeated more that 1 time.", lexicrow[0]);
	}

	i++;

	while (i < lexicrow.size() && lexicrow[i]->getKey() == L"Operator")
	{
		wstring opvalue = lexicrow[i]->getValue();
		if (opvalue == L"*")
		{
			if (isRef)
			{
				Log::pushError(ErrorMessage::syntaxError + L"Reference cannot be over pointer.", lexicrow[i]);
			}
			countOfPtrs++;
		}
		else if (opvalue == L"&")
		{
			if (!isRef)
			{
				isRef = true;
			}
			else 
			{
				Log::pushError(ErrorMessage::syntaxError + L"Rvalue reference does not supported.", lexicrow[i]);
			}
		}
		else
		{
			Log::pushError(ErrorMessage::syntaxError + L"\'*\' or \'&\' expected.", lexicrow[i]);
		}
		i++;
	}

	if (i < lexicrow.size())
	{
		if (!expectKey(lexicrow[i], L"Identifier"))
		{
			Log::pushError(ErrorMessage::syntaxError + L"Identifier expected.", lexicrow[i]);
			return false;
		}
		i++;
		if (i < lexicrow.size() && expectValue(lexicrow[i], L";"))
		{

			// TODO:
			//Variable* var = new Variable;
			MetaInfo.getMetaStack().back().pushVariable();
		}
		if (i < lexicrow.size() && expectValue(lexicrow[i], L"="))
		{
			// TODO reverse polish notation in Tree
			wcout << "There comes value definition" << endl;
		}
		return true;
		//if (expectValue(lexicrow[i + 1], L";") || expectValue(lexicrow[i + 1], L"="))
		//{
		//	//return true;
		//}
		//else
		//{
		//	Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'=\' expected.", lexicrow[i]);
		//}
	}
	return false;
}


bool SyntaxAnalyzer::isDeclarator(vector<LexicalUnit*>& lexicrow)
{
	if (isClassDeclarator(lexicrow))
	{
		if (isDefinition(lexicrow))
		{
			wcout << L"Defined new type:" << lexicrow[1]->getValue() << endl;
			// TODO: Push Scope
			//return true;
		}
		else
		{
			wcout << L"Declared new type:" << lexicrow[1]->getValue() << endl;
		}
		return true;
	}
	else if (isVariableDeclarator(lexicrow))
	{
		wcout << L"Declared new variable: ";
		for (size_t i = 0; i < lexicrow.size(); i++)
		{
			wcout << lexicrow[i]->getValue() << L" ";
		}
		wcout << endl;
	}

	return false;
}