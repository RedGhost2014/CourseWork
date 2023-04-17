#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "File.hpp"

using namespace std;

class LexicalUnit;

class AbstractLexicTree
{
public:
	AbstractLexicTree();
	~AbstractLexicTree();

	using Action = std::function<void(LexicalUnit*)>;

	void print(wstring filename);

	void push(LexicalUnit* su);
	void push(File* f);
	void pop();

	LexicalUnit* getHead();

	void checkBracketBalance();

private:
	void recurse(LexicalUnit* v, Action action);
	void tree(LexicalUnit* v, std::wofstream&);

	vector<vector<LexicalUnit*>*> stack;

	vector<LexicalUnit*> roundBracketStack;
	vector<LexicalUnit*> squareBracketStack;
	vector<LexicalUnit*> figureBracketStack;

	LexicalUnit* head;
};


class LexicalUnit
{
public:
	LexicalUnit();
	LexicalUnit(const LexicalUnit&);
	LexicalUnit(wstring s);
	LexicalUnit(size_t);
	~LexicalUnit();

	wstring getValue();
	wstring getKey();
	size_t getLine();
	wstring getFilename();

	void setValue(wstring s);
	void setKey(wstring s);
	void setFilename(wstring s);

	vector<LexicalUnit*>& getTree();

protected:
	vector<LexicalUnit*> m_tree;
	wstring m_key;
	wstring m_value;
	wstring filename;
	size_t line;
};

