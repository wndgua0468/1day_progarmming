#include <iostream>

using namespace std;

class MyClass
{
private:
	char *str;
public:
	MyClass(const char *aStr)
	{
		str = new char[strlen(aStr)+1];
		strcpy(str, aStr);
	}
	~MyClass() {
		delete []str;
		cout << "~MyClass() called!" << endl;
	}
	void ShowData()
	{
		cout << "str: " << str << endl;
	}
};

int main()
{
	MyClass mc1("MyClass!");
	MyClass mc2 = mc1;

	mc1.ShowData();
	mc2.ShowData();
	return 0;
}
