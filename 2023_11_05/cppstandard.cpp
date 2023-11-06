#include<iostream>
using namespace std;

	namespace kitae
	{
		int f() { return 1; }
		int m() { return -1; }
	}

	int main()
	{
		std::cout << "Hi" << std::endl << "Hello" << std::endl;

		int n = kitae::f();
		int m = kitae::m();

		cout << "Hello" << endl;

		int width, height;

		cout << "너비와 높이 입력 : ";
		cin >> width >> height;
		return 0;
	}
