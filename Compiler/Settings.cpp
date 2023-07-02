#include "Settings.hpp"
#include "Utils.hpp"
#include "File.hpp"

Settings& Settings::parse(wchar_t* argv[], size_t argc)
{
	if (argc == 1)
	{
		cout << "My compiler." << endl;
		exit(0);
	}

	vector<wstring> possibleExtensions = { L".c", L".i" };

	filter(argv, argc);

	for (size_t i = 0; i < names.size(); i++)
	{
		File* f = new File();

		if (Utils::isFileExists(names[i]))
		{
			f->setName(names[i]);
			f->setExistState(true);
			sourceFiles.push_back(f);
			continue;
		}

		bool found = false;

		for (wstring ext : possibleExtensions)
		{
			if (Utils::isFileExists(names[i] + ext))
			{
				f->setName(names[i] + ext);
				f->setExistState(true);

				sourceFiles.push_back(f);
				found = true;
				break;
			}
		}

		if (found)
			continue;
		
		f->setName(names[i]);
		f->setExistState(false);
		sourceFiles.push_back(f);
	}
	return *this;
}

void Settings::filter(wchar_t* argv[], size_t argc)
{
	for (size_t i = 1; i < argc; i++)
	{
		wstring argument = argv[i];
		if (argument[0] == L'-')
		{
			this->flags.push_back(argument);
		}
		else
		{
			this->names.push_back(argument);
		}
	}
}

vector<File*> Settings::getSourceFiles()
{
	return sourceFiles;
}


bool Settings::isContainFlag(wstring flag)
{
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (flag == flags[i])
		{
			return true;
		}
	}
	return false;
}

void Settings::dbgprint()
{
	wcout << L"Flags: ";
	for (size_t i = 0; i < flags.size(); i++)
	{
		wcout << flags[i] << L", ";
	}
	wcout << endl;

	wcout << L"Names: ";
	for (size_t i = 0; i < names.size(); i++)
	{
		wcout << names[i] << L", ";
	}
	wcout << endl;
}