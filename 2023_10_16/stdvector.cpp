#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class A {
private:
    int num;
    string name;

public:
    A(int i, string s) : num(i), name(s) {}
};

int main() {
    vector<int> v = { 1, 2, 3, 4 };

    for_each(v.begin(), v.end(), [](int n) {
        cout << n << endl; // output: 1 2 3 4
        });

    for_each(v.rbegin(), v.rend(), [](int n) {
        cout << n << endl; // output: 4 3 2 1
        });

    ////////////////////////////////////////////////////

    vector<int>::iterator itor = v.begin();

    for (; itor != v.end(); itor++)
        cout << *itor << endl; // output: 1 2 3 4

    vector<int>::reverse_iterator itor2 = v.rbegin();

    for (; itor2 != v.rend(); itor2++)
        cout << *itor2 << endl; // output: 4 3 2 1

    ////////////////////////////////////////////////////

    vector<int> v2 = { 1, 2, 3, 4 };

    cout << v2.front() << endl;    // output: 1
    cout << v2.back() << endl;     // output: 4
    cout << v2.at(1) << endl;      // output: 2
    cout << v2[2] << endl;         // output: 3

    ////////////////////////////////////////////////////

    vector<int> v3;

    v3.push_back(10);
    v3.push_back(20);              // v3 = {10, 20}

    v3.insert(v3.begin() + 1, 100); // v3 = {10, 100, 20}

    v3.pop_back();                 // v3 = {10, 100}

    v3.emplace_back(1);            // v3 = {10, 100, 1}
    v3.emplace_back(2);            // v3 = {10, 100, 1, 2}
    v3.emplace(v3.begin() + 2, -50); // v3 = {10, 100, -50, 1, 2}

    v3.erase(v3.begin() + 1);       // v3 = {10, -50, 1, 2}
    v3.resize(6);                   // v3 = {10, -50, 1, 2, 0, 0}
    v3.clear();                     // v3 = empty()

    ////////////////////////////////////////////////////

    vector<A> v4;
    A a(1, "hi");

    v4.push_back(a);
    v4.emplace_back(1, "hi");

    ////////////////////////////////////////////////////

    v4.clear();
    cout << v4.capacity() << endl; // output: 0

    v4.shrink_to_fit();
    cout << v4.capacity() << endl; // output: 0

    ////////////////////////////////////////////////////

    cout << v4.size() << endl;      // output: 0
    cout << v4.capacity() << endl;  // output: 0

    v4.reserve(6);
    cout << v4.capacity() << endl;  // output: 6
    cout << v4.max_size() << endl;  // output: (system-dependent)

    v4.shrink_to_fit();
    cout << v4.capacity() << endl;  // output: 0

    cout << v4.empty() << endl;     // output: true
    v4.clear();
    cout << v4.empty() << endl;     // output: true

    return 0;
}
