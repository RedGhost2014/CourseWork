#include "Preprocessor.hpp"

std::vector<std::wstring> parse(std::wstring s, std::wstring delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::wstring token;
	std::vector<std::wstring> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::wstring::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(s.substr(pos_start));
	return res;
};

Preprocessor& Preprocessor::open(wstring filename)
{
	originalName = filename;
	input.open(originalName);
	preprocessedName = originalName.substr(0, originalName.find_last_of(L'.') + 1) + L"i";
	this->output.open(preprocessedName, std::ios_base::out);
	return *this;
}

Preprocessor& Preprocessor::close()
{
	if (input.is_open())
	{
		input.close();
	}

	if (output.is_open())
	{
		output.close();
	}
	return *this;
}

wstring Preprocessor::getPreprocessedFilename()
{
	return preprocessedName;
}

Preprocessor& Preprocessor::parse()
{
	wstring str = L"";

	bool AccessToWriteByDefinitions = true;
	bool isLongComment = false;

	while (!input.eof())
	{
		std::getline(input, str);
		replaceDefines(str);

		if (str.find(L"//") != str.npos)
		{
			str = str.substr(0, str.find(L"//"));
		}

		if (str.find(L"/*") != str.npos && str.find(L"*/") != str.npos)
		{
			str = str.substr(0, str.find(L"/*")) + str.substr(str.find(L"*/") + 2);
		}
		else if (str.find(L"/*") != str.npos)
		{
			isLongComment = true;
			str = str.substr(0, str.find(L"/*"));
		}
		else if (str.find(L"*/") != str.npos)
		{
			isLongComment = false;
			str = str.substr(str.find(L"*/") + 2);
		}

		if (isLongComment)
		{
			str = L"";
		}

		

		

		if (str.find(L"#include") != str.npos)
		{

		}
		else if (str.find(L"#define") != str.npos)
		{
			if (containBrackets(str))
			{
				wstring result = retriveBrackets(str);
				result = erase(result, L" ");
				vector<wstring> v = ::parse(result, L",");
				str = dropBrackets(str);
				functiondefs.push_back(pair<vector<wstring>, wstring>(v, str));
			}
			else
			{
				vector<wstring> v = ::parse(str, L" ");
				if (v.size() == 2)
					macroses.push_back(v[1]);
				if (v.size() == 3)
					basicdefs.push_back(pair<wstring, wstring>(v[1], v[2]));
			}
		}
		else if (str.find(L"#ifdef") != str.npos)
		{
			bool found = false;
			vector<wstring> v = ::parse(str, L" ");
			for (size_t i = 0; i < macroses.size(); i++)
			{
				for (size_t j = 0; j < v.size(); j++)
				{
					if (macroses[i] == v[j])
					{
						found = true;
						AccessToWriteByDefinitions = true;
						break;
					}
				}
				if (found)
					break;
			}
			if (!found)
				AccessToWriteByDefinitions = false;
		}
		else if (str.find(L"#else") != str.npos)
		{
			AccessToWriteByDefinitions = !AccessToWriteByDefinitions;
		}
		else if (str.find(L"#endif") != str.npos)
		{
			AccessToWriteByDefinitions = true;
		}
		else
		{
			if (AccessToWriteByDefinitions)
				output << str.c_str() << endl;
		}
	}
	return *this;
}

wstring Preprocessor::retriveBrackets(wstring& base)
{
	//wstring result = L"";

	size_t left = base.find(L'(') + 1;
	size_t right = base.find(L')');
	if (left != base.npos && right != base.npos && left < right)
	{
		return base.substr(left, right - left);
	}
	else return wstring(L"");
}

wstring Preprocessor::dropBrackets(wstring& base)
{
	wstring res = retriveBrackets(base);
	return base.substr(base.find(res) + res.length() + 2);
}

wstring Preprocessor::erase(wstring& base, wstring symbols)
{
	wstring result = L"";
	for (size_t i = 0; i < base.length(); i++)
	{
		bool add = true;
		for (size_t j = 0; j < symbols.length(); j++)
		{
			if (base[i] == symbols[j])
			{
				add = false;
				break;
			}
		}
		if (add)
			result += base[i];
	}
	return result;
}

bool Preprocessor::containBrackets(wstring& base)
{
	if (base.find(L'(') != base.npos && base.find(L')') != base.npos)
	{
		return true;
	}
	else return false;
}

void Preprocessor::replaceDefines(wstring& str)
{
	for (size_t i = 0; i < basicdefs.size(); i++)
	{
		size_t res = str.find(reinterpret_cast<wstring&>(basicdefs[i].first));
		while (res != str.npos)
		{
			str.replace(res, basicdefs[i].first.length(), basicdefs[i].second);
			res = str.find(reinterpret_cast<wstring&>(basicdefs[i].first));
		}
	}

	for (size_t i = 0; i < functiondefs.size(); i++)
	{

	}
}