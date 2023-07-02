#pragma once
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class BaseFlow
{
public:
	virtual BaseFlow& operator<<(T _data) { insertToFlow(_data); return *this; };

private:
	virtual void insertToFlow(T& _data) {};
};

template <typename T>
class StorageFlow : public BaseFlow<T>
{
public:
	StorageFlow() = default;
	~StorageFlow() = default;
	void insertToFlow(T& _data) override
	{
		storage.push_back(_data);
	};
	vector<T>& getStorage() { return storage; };

private:
	vector<T> storage;
};

template <typename T>
class ConsoleFlow : public BaseFlow<T>
{
public:
	ConsoleFlow() = default;
	~ConsoleFlow() = default;

	void insertToFlow(T& _data) override
	{
		wcout << _data;
	};
};

template <typename T>
class FileFlow : public BaseFlow<T>
{
public:
	FileFlow(wstring name) { open(name); };
	~FileFlow() = default;

	void open(wstring name) { output.open(name); };
	void close() { output.close(); };

	void insertToFlow(T& _data) override
	{
		output << _data;
	};
private:
	wofstream output;
};