// P.S:
// After i wrote half of the project i realized that my basic idea of 
// "why am i supposed to create whole syntax tree if it affects a lot only in expressions, lets do it just by compare strings" 
// was not an absolute error, because as you can see - "it just works"
// but i barely not won as much from it as i expected when im decided to write a compiler from absolute scratch.
// 
// I mean its cool to do it in c-style just by comparing strings, but later i remembered about dynamic casts and polymorphism and it changes to them
// and it became look better, complex, and more object-oriented. 
// But underneath there always be that mech of comparing arrays by specific values, despite strings are them or class objects.
// There will be compares by == or downcasts because thats just how compiler & syntax analyze works no matter what, and you have to do this.

// Maybe someday i will rewrite half of this to make completely full AST as it supposed to be in every compiler, 
// and it will even become a part of cpp compiler but we'll see...

#include <iostream>
#include <Windows.h>
#include "File.hpp"
#include "Settings.hpp"
#include "Tokenizer.hpp"
#include "ALT.hpp"
#include "AST.hpp"
#include "Log.hpp"
#include "MetaInfo.hpp"
#include "Preprocessor.hpp"
#include "SyntaxAnalyzer.hpp"
#include "CodeGenerator.hpp"
#include "Flow.hpp"

wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];

	size_t num = 0;

	mbstowcs_s(&num, wc, cSize, c, cSize);
	return wc;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	_wsetlocale(LC_ALL, L"Rus");

	// "--ltrace" - trace lexic tree
	// "--strace" - trace syntax tree
	// "--fixit" - fix semicolon

	const wchar_t* argvectors[] =
	{
		L"Compiler.exe",
		L"somecode.c",
		//L"somecode2.c",
		L"--ltrace",
		L"--strace",
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

	vector<File*> inputFiles = settings.parse((WCHAR**)argvectors, count).getSourceFiles();

	Preprocessor preprocessor;

	for (File* f : inputFiles)
	{
		f->setName(preprocessor.open(f->getName()).parse().close().getPreprocessedFilename());
	}

	Tokenizer t;
	vector<AbstractLexicTree*> trees;
	
	for (File* f : inputFiles)
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

	if (settings.isContainFlag(L"--ltrace"))
	{
		wofstream f;
		f.open(L"astrace.txt");
		f.close();
		for (AbstractLexicTree* alt : trees)
		{
			alt->print(L"cout");
			alt->print(L"astrace.txt");
		}
		wcout << "\n\n\n" << endl;
	}

	
	SyntaxAnalyzer SA;

	for (size_t i = 0; i < inputFiles.size(); i++)
	{
		SA.addSourceFileToAST(inputFiles[i]);
		SA.consumeLexicTree(trees[i]);
	}

	wcout << "\n\n\n" << endl;

	if (settings.isContainFlag(L"--strace"))
	{
		SA.getTree()->print();
	}

	wcout << "\n\n\n" << endl;
	Log::print(L"cout");
	Log::print(L"log.txt");

	StorageFlow<wstring>* storage = new StorageFlow<wstring>();

	CodeGenerator codeGenerator;

	Mangler* m = new Mangler();

	codeGenerator.bindOutputFlowController(storage);
	codeGenerator.bindMangler(m);
	codeGenerator.generateCodeBasedOnAST(SA.getTree());

	FileFlow<wstring>* file = new FileFlow<wstring>(L"mycodegen.asm");
	ConsoleFlow<wstring>* console = new ConsoleFlow<wstring>();

	for (size_t i = 0; i < storage->getStorage().size(); i++)
	{
		*file << storage->getStorage()[i];
		*console << storage->getStorage()[i];
	}
	/*
	* @echo off
	echo.
	ml /nologo /c /Gd test.asm
	echo.
	link /nologo /machine:X86 /subsystem:console /entry:_main_8f993366 /DYNAMICBASE /NXCOMPAT test.obj
	echo on
	*/
	file->close();
	return 0;
}