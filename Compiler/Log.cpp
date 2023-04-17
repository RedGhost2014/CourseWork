#include "Log.hpp"



class Error
{
public:
	Error(wstring _file, wstring error, size_t line);
	~Error() = default;

	size_t getLine();
	wstring what();
	wstring destination();

private:
	wstring _what;
	wstring file;
	size_t line;
};


Error::Error(wstring _file, wstring error, size_t _line)
{
	_what = error;
	line = _line;
	file = _file;
}

size_t Error::getLine()
{
	return line;
}

wstring Error::what()
{
	return _what;
}

wstring Error::destination()
{
	return file;
}

class BaseLog
{
public:
	BaseLog() = default;
	~BaseLog() = default;

	void pushError(wstring _file, wstring error, size_t _line);
	void pushWarning(wstring _file, wstring warning, size_t _line);
	void print(wstring filename);
	size_t getErrorsCount();
	size_t getWarningsCount();
private:
	vector<Error> errors;
	vector<Error> warnings;
};

BaseLog* baseLogSystem = new BaseLog();


size_t Log::getErrorsCount()
{
	return baseLogSystem->getErrorsCount();
}

size_t Log::getWarningsCount()
{
	return baseLogSystem->getWarningsCount();
}

void Log::pushError(wstring _file, wstring error, size_t _line)
{
	baseLogSystem->pushError(_file, error, _line);
}

void Log::pushWarning(wstring _file, wstring warning, size_t _line)
{
	baseLogSystem->pushWarning(_file, warning, _line);
}

void Log::print(wstring filename)
{
	baseLogSystem->print(filename);
}




size_t BaseLog::getErrorsCount()
{
	return errors.size();
}

size_t BaseLog::getWarningsCount()
{
	return warnings.size();
}

void BaseLog::pushError(wstring _file, wstring error, size_t _line)
{
	errors.push_back(Error(_file, error, _line));
}


void BaseLog::pushWarning(wstring _file, wstring warning, size_t _line)
{
	warnings.push_back(Error(_file, warning, _line));
}

void BaseLog::print(wstring filename)
{
	wofstream* output = nullptr;

	if (filename == L"cout")
	{
		output = (wofstream*)&wcout;
	}
	else
	{
		output = new wofstream;
		output->open(filename);
		if (!output->is_open())
			return;
	}

	wofstream& out = *output;

	out << "\n\n-------------------\n\n";

	out << "Total Errors: " << errors.size() << "\n" << endl;

	for (size_t i = 0; i < errors.size(); i++)
	{
		out << i + 1 << ") " << "<" << errors[i].destination() << ">: (" << errors[i].getLine() << "): \t" << errors[i].what() << endl;
	}

	out << "\n\n-------------------\n\n";

	out << "Total Warnings: " << warnings.size() << "\n" << endl;

	for (size_t i = 0; i < warnings.size(); i++)
	{
		out << i + 1 << ") " << "<" << warnings[i].destination() << ">: (" << warnings[i].getLine() << "): \t" << warnings[i].what() << endl;
	}

	out << "\n\n-------------------\n\n";

	if (filename != L"cout")
	{
		output->close();
		delete output;
	}
}