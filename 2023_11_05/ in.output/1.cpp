#include<iostream>
using namespace std;

int main() {
	string s;
	double n[5];
	double max;

	cout << "5개의 실수를 입력하라 >>";

	for (int i = 0; i < 5; i++) {
		cin >> n[i];
		if (i == 0)max = n[i];
		if (n[i] > max)max = n[i];

	}
	cout << "가장 큰 수는" << max << endl;
}
