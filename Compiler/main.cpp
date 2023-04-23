#include <Windows.h>
#include "File.hpp"
#include "Settings.hpp"
#include "Tokenizer.hpp"
#include "ALT.hpp"
#include "AST.hpp"
#include "Log.hpp"
#include "MetaInfo.hpp"
#include "SyntaxAnalyzer.hpp"

wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];

	size_t num = 0;
	
	mbstowcs_s(&num, wc, cSize, c, cSize);

	return wc;
}

//void f(char&**);

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	_wsetlocale(LC_ALL, L"Rus");

	// "--astrace" - ¬ывод лексического дерева разбора в консоль
	// "--fixit" - 


	const wchar_t* argvectors[] =
	{
		L"Compiler.exe",
		L"test.c",
		//L"Hello World.i",
		L"somecode.c",
		L"--astrace",
	};
	size_t count = sizeof(argvectors) / sizeof(wchar_t*);
	
	/*WCHAR** argvectors = new WCHAR*[argc];
	for (size_t i = 0; i < argc; i++)
	{
		argvectors[i] = GetWC(argv[i]);
	}
	size_t count = argc;
	*/
	Settings settings;


	vector<File*> v = settings.parse((WCHAR**)argvectors, count).getSourceFiles();
	settings.dbgprint();

	for (File* f : v)
	{
		f->dbgprint();
		cout << endl;
	}

	Tokenizer t;
	vector<AbstractLexicTree*> trees;

	for (File* f : v)
	{
		AbstractLexicTree* alt = new AbstractLexicTree;
		trees.push_back(alt);
		alt->push(f);
		t.bind(f).open();
		while (!t.eof())
		{
			LexicalUnit* result = t.getNextToken();
			if (result)
			{
				alt->push(result);
			}
		}
		t.close();
	}

	for (AbstractLexicTree* alt : trees)
	{
		alt->checkBracketBalance();
	}

	if (settings.isContainFlag(L"--astrace"))
	{
		wofstream f;
		f.open(L"astrace.txt");
		f.close();
		for (AbstractLexicTree* alt : trees)
		{
			alt->print(L"cout");
			alt->print(L"astrace.txt");
		}
	}


	wcout << "\n\n\n" << endl;

	SyntaxAnalyzer SA;
	SA.consumeLexicTree(trees[0]);
	
	Log::print(L"cout");
	Log::print(L"log.txt");

	return 0;
}