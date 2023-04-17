#include <iostream>
#include <fstream>
#include "File.hpp"
#include "ALT.hpp"

class ITokenizer
{
public:
	virtual LexicalUnit* getNextToken() = 0;
};

class Tokenizer : ITokenizer
{
public:
	Tokenizer() = default;
	~Tokenizer() = default;

	static wstring alphabet;
	static wstring digits;
	static wstring spaces;
	static vector<wstring> keywords;

	bool isBelongToAlphabet(wchar_t c, wstring alphabet);
	bool isBelongToAlphabet(wstring literal, vector<wstring>& alphabet);

	bool is_open();
	void open();
	void close();
	Tokenizer& bind(File* f);
	bool eof();
	void unbind();

	LexicalUnit* getNextToken() override;

private:
	static size_t currentString;
	File* m_inputfile;
	std::wifstream m_inputstream;
};
