int testfunction(int a, int b);

int main()
{
	int a = -5;
	int b = -(a - 5);

	char d = 123;

	int e = testfunction(a++ + b, 123 - d);
}

int testfunction(int a, int b)
{
	int c = a + b;
	return c;
}
