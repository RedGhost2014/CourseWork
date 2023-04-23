#include <Windows.h>
#include "ALT.hpp"
#include "Log.hpp"

AbstractLexicTree::AbstractLexicTree()
{
	this->head = new LexicalUnit(L"Head");
}

LexicalUnit* AbstractLexicTree::getHead()
{
	return head;
}

void AbstractLexicTree::recurse(LexicalUnit* su, Action action)
{
	vector<LexicalUnit*>& v = su->getTree();
	for (size_t i = 0; i < v.size(); i++)
	{
		action(v[i]);
		recurse(v[i], action);
	}
}


void AbstractLexicTree::tree(LexicalUnit* su, wofstream& stream)
{
	static size_t recursionDepthLevel = 0;
	vector<LexicalUnit*>& v = su->getTree();

	for (size_t i = 0; i < v.size(); i++)
	{
		stream << v[i]->getLine() << "  |";
		for (size_t i = 0; i < recursionDepthLevel; i++)
		{
			stream << "\t";
		}

		stream << v[i]->getValue() << "\t   <" << v[i]->getKey() << ">" << endl;

		recursionDepthLevel++;
		tree(v[i], stream);
		recursionDepthLevel--;
	}
}

void AbstractLexicTree::print(wstring filename)
{
	if (filename == L"cout")
	{
		wofstream* test = (wofstream*)&wcout;
		tree(head, *test);
	}
	else
	{
		wofstream output(filename, std::ios::app);

		tree(head, output);

		output.close();
	}
}


AbstractLexicTree::~AbstractLexicTree()
{
	recurse(head, [](LexicalUnit* su)
		{
			delete su;
		});
	delete head;
}


void AbstractLexicTree::push(File* f)
{
	LexicalUnit* sourcefile = new LexicalUnit(f->getName());
	sourcefile->setKey(L"Source file");
	head->getTree().push_back(sourcefile);
	stack.push_back(&(head->getTree().back()->getTree()));
}

void AbstractLexicTree::pop()
{
	stack.pop_back();
}

void AbstractLexicTree::checkBracketBalance()
{
	recurse(head, [&](LexicalUnit* su)
		{
			wstring element = su->getValue();
			if (element == L"{")
			{
				figureBracketStack.push_back(su);
			}
			else if (element == L"[")
			{
				squareBracketStack.push_back(su);
			}
			else if (element == L"(")
			{
				roundBracketStack.push_back(su);
			}
			else if (element == L"}")
			{
				if (figureBracketStack.empty())
				{
					Log::pushError(su->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Open_Figure_Missed, su->getLine());
				}
				else
				{
					figureBracketStack.pop_back();
				}
			}
			else if (element == L"]")
			{
				if (squareBracketStack.empty())
				{
					Log::pushError(su->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Open_Square_Missed, su->getLine());
				}
				else
				{
					squareBracketStack.pop_back();
				}
			}
			else if (element == L")")
			{
				if (roundBracketStack.empty())
				{
					Log::pushError(su->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Open_Round_Missed, su->getLine());
				}
				else
				{
					roundBracketStack.pop_back();
				}
			}
		}
	);

	if (figureBracketStack.size() != 0)
	{
		for (size_t i = 0; i < figureBracketStack.size(); i++)
		{
			Log::pushError(figureBracketStack[i]->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Close_Figure_Missed, figureBracketStack[i]->getLine());
		}
	}

	if (squareBracketStack.size() != 0)
	{
		for (size_t i = 0; i < squareBracketStack.size(); i++)
		{
			Log::pushError(squareBracketStack[i]->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Close_Square_Missed, squareBracketStack[i]->getLine());
		}
	}

	if (roundBracketStack.size() != 0)
	{
		for (size_t i = 0; i < roundBracketStack.size(); i++)
		{
			Log::pushError(roundBracketStack[i]->getFilename(), ErrorMessage::syntaxError + ErrorMessage::bracket_Close_Round_Missed, roundBracketStack[i]->getLine());
		}
	}
}

void AbstractLexicTree::push(LexicalUnit* su)
{
	wstring element = su->getValue();
	su->setFilename(head->getTree().back()->getValue());
	if (element == L"{" || element == L"[" || element == L"(")
	{
		stack.back()->push_back(su);
		stack.push_back(&su->getTree());
	}
	else if (element == L"}" || element == L"]" || element == L")")
	{
		if (stack.size() > 1)
			stack.pop_back();

		stack.back()->push_back(su);
	}
	else
	{
		stack.back()->push_back(su);
	}

}











LexicalUnit::LexicalUnit() {}
LexicalUnit::LexicalUnit(const LexicalUnit& rhs) 
{
	m_tree = rhs.m_tree;
	m_key = rhs.m_key;
	m_value = rhs.m_value;
	filename = rhs.filename;
	line = rhs.line;
}
LexicalUnit::LexicalUnit(wstring s)
{
	setValue(s);
}
LexicalUnit::LexicalUnit(size_t currentString)
{
	line = currentString;
}

LexicalUnit::~LexicalUnit() {}

vector<LexicalUnit*>& LexicalUnit::getTree()
{
	return m_tree;
}

void LexicalUnit::setValue(wstring s)
{
	m_value = s;
}

wstring LexicalUnit::getValue()
{
	return m_value;
}

wstring LexicalUnit::getFilename()
{
	return filename;
}

void LexicalUnit::setFilename(wstring filename)
{
	this->filename = filename;
}

void LexicalUnit::setKey(wstring s)
{
	m_key = s;
}

wstring LexicalUnit::getKey()
{
	return m_key;
}

size_t LexicalUnit::getLine()
{
	return line;
}

