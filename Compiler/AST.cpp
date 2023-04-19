#include "AST.hpp"

class CategoryDeterminer
{
public:
	CategoryDeterminer() = default;

	bool isDeclarator(vector<LexicalUnit*>& lexicrow);
	bool isKeyword(LexicalUnit*);
};

bool CategoryDeterminer::isDeclarator(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow.size() == 0)
		return false;
	if (lexicrow[0]->getKey() == L"Keyword") // TODO:: isType
	{
		// TODO: MetaInfo
		if (lexicrow[1]->getKey() == L"Identifier")
		{
			return true;
		}
	}
	return false;
}

bool CategoryDeterminer::isKeyword(LexicalUnit* unit)
{
	return unit->getKey() == L"Keyword";
}

void AbstractSyntaxTree::bypassLexicTree(LexicalUnit* head)
{
	vector<LexicalUnit*> parserow;

	vector<LexicalUnit*>& currentLexicalRow = head->getTree();

	size_t i = 0;
	while (i < currentLexicalRow.size())
	{
		wstring tmp = currentLexicalRow[i]->getValue();
		parserow.push_back(currentLexicalRow[i]);
		if (tmp == L"{")
		{
			//isDetermine = true;
			printLexicRow(parserow);
			determineCategory(parserow);
			parserow.clear();
		}
		if (tmp == L";")
		{
			printLexicRow(parserow);
			determineCategory(parserow);
			parserow.clear();
		}
		i++;
	}
	if (!parserow.empty())
	{
		printLexicRow(parserow);
		determineCategory(parserow);
	}
}

void AbstractSyntaxTree::consumeLexicTree(AbstractLexicTree* alt)
{
	//alt->getHead();
	bypassLexicTree(alt->getHead()->getTree()[0]);
}

void AbstractSyntaxTree::printLexicRow(vector<LexicalUnit*>& lexicrow)
{
	static int z = 0;
	wcout << L"lexicrow[" << z << L"] = ";
	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wcout << lexicrow[i]->getValue() << " ";
	}
	wcout << endl;
	z++;
}

void AbstractSyntaxTree::determineCategory(vector<LexicalUnit*>& lexicrow)
{
	static CategoryDeterminer cd;


	if (cd.isKeyword(lexicrow[0]))
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

void AbstractSyntaxTree::adjustTypes(vector<LexicalUnit*>& lexicrow)
{
	vector<LexicalUnit*> retyped;
	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wstring tmp = lexicrow[i]->getKey();
		if (tmp == L"Type")
		{
			wstring newType = lexicrow[i]->getValue();
			LexicalUnit* lu = lexicrow[i];
			while (lexicrow[i + 1]->getValue() == L"*")
			{
				newType += L"*";
				i++;
			}
			lu->setValue(newType);
			retyped.push_back(lu);
		}
		else
		{
			retyped.push_back(lexicrow[i]);
		}
	}
	lexicrow = retyped;
}