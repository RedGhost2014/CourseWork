#include "Tokenizer.hpp"

using namespace std;

wstring Tokenizer::alphabet = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
wstring Tokenizer::digits = L"01234567890";
wstring Tokenizer::spaces = L" \n\t";
size_t Tokenizer::currentString = 1;


vector<wstring> Tokenizer::keywords = {

	// Pure keywords
	L"break",L"case",L"while",L"for",L"goto",L"return",L"continue",L"default",
	L"if",L"else",L"extern", L"do",L"enum",L"sizeof", L"switch", L"typedef", L"union", L"namespace", L"true", L"false",
	L"nullptr",
	// Types
	L"auto",L"void", L"char", L"int",L"short", L"long",L"float",L"double", L"struct", L"operator", L"class"

	// Qualifiers
	L"const", L"register", L"signed", L"static", L"unsigned", L"volatile", L"nodiscard", L"immutable", L"public", L"private"
};

Tokenizer& Tokenizer::bind(File* f)
{
	this->m_inputfile = f;
	return *this;
}

void Tokenizer::unbind()
{
	this->m_inputfile = nullptr;
}

bool Tokenizer::is_open()
{
	return m_inputstream.is_open();
}

bool Tokenizer::eof()
{
	return m_inputstream.eof();
}

void Tokenizer::open()
{
	if (this->m_inputfile != nullptr)
	{
		m_inputstream.open(this->m_inputfile->getName());
	}
	currentString = 1;
}

void Tokenizer::close()
{
	m_inputstream.close();
}

bool Tokenizer::isBelongToAlphabet(wchar_t c, wstring alphabet)
{
	for (size_t i = 0; i < alphabet.length(); i++)
	{
		if (c == alphabet[i])
			return true;
	}
	return false;
}

bool Tokenizer::isBelongToAlphabet(wstring literal, vector<wstring>& alphabet)
{
	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (literal == alphabet[i])
			return true;
	}
	return false;
}

LexicalUnit* Tokenizer::getNextToken()
{
	LexicalUnit* su = nullptr;
	wstring token = L"";

	wchar_t c = NULL;
	c = m_inputstream.peek();

	if (c == 65535)
		return su;

	// Any spaces
	if (isBelongToAlphabet(c, spaces))
	{
		if (c == '\n')
			currentString++;
		token += c;
		m_inputstream.get();
		return su;
	}

	// Literal operator
	if (c == '\"')
	{
		c = m_inputstream.get();
		token += c;
		c = m_inputstream.peek();

		size_t slashInRow = 0;

		while (!m_inputstream.eof())
		{
			if (c == '\\')
				slashInRow++;
			else
				slashInRow = 0;

			c = m_inputstream.get();

			token += c;
			if (c == '\"' && slashInRow % 2 == 0)
			{
				break;
			}
		}

		su = new LexicalUnit(currentString);
		su->setKey(L"String Literal");
		su->setValue(token);
		return su;
	}

	if (c == '\'')
	{
		c = m_inputstream.get();
		token += c;
		c = m_inputstream.peek();

		while (!m_inputstream.eof())
		{
			c = m_inputstream.get();

			token += c;
			if (c == '\'')
			{
				break;
			}
		}

		su = new LexicalUnit(currentString);
		su->setKey(L"Character Literal");
		su->setValue(token);
		return su;
	}





	// Pure Identifier
	if (isBelongToAlphabet(c, alphabet))
	{
		while (!m_inputstream.eof())
		{
			c = m_inputstream.peek();
			if (isBelongToAlphabet(c, alphabet) || isBelongToAlphabet(c, digits))
			{
				token += c;
				m_inputstream.get();
			}
			else break;
		}

		su = new LexicalUnit(currentString);

		if (isBelongToAlphabet(token, keywords))
		{
			su->setKey(L"Keyword");
		}
		else
		{
			su->setKey(L"Identifier");
		}
		su->setValue(token);
		return su;
	}


	// Number/Identifier
	else if (isBelongToAlphabet(c, digits))
	{
		bool wasFloat = false;
		while (!m_inputstream.eof())
		{
			c = m_inputstream.peek();
			if (isBelongToAlphabet(c, digits))
			{
				token += c;
				m_inputstream.get();
			}
			else if (c == '.' && !wasFloat)
			{
				token += c;
				m_inputstream.get();
				wasFloat = true;
			}
			else break;
		}

		su = new LexicalUnit(currentString);
		su->setKey(L"Number");
		su->setValue(token);
		return su;
	}
	else if (c == ';')
	{
		c = m_inputstream.get();
		token += c;

		su = new LexicalUnit(currentString);
		su->setKey(L"End line");
		su->setValue(token);
		return su;
	}
	else
	{
		c = m_inputstream.get();
		token += c;

		su = new LexicalUnit(currentString);
		su->setKey(L"Operator");
		su->setValue(token);
		return su;
	}
}
