
int i;
int* g = (int*)malloc(sizeof(int*) * 5);

typedef int Word;

typedef struct Car
{
	int speed;
	char* name;
} Car;

namespace asd
{
	size_t x = 5;
}

int operator~(int rhs)
{
	return rhs - 3;
}

int main(char* arg)
{
	int a = 123;
	int* b = &a;
	b[5] = 0x1337;

	if (a == 5)
	{
		printf("Somestring");
	}
	else if (a == 6)
	{
		printf("Another Somestring");
	}

	for (size_t i = 0; i < 15; i++)
	{
		printf("%d", i);
		if (i == 5)
			break;
	}

	s = sizeof(5);

	Type* t = nullptr;

	char c = 'c';
	double x = 123.123;
	const char* str = "Hello \\\" world";
	f(5);
	{
		int a1 = 5;
	}
	return 0;
}