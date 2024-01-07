#include <bits/stdc++.h>

using namespace std;

int n, l;
vector<pair<double, double>> v, res;

double ccw(pair<double, double> p1, pair<double, double> p2, pair<double, double> p3) {
	double a = p1.first * p2.second + p2.first * p3.second + p3.first * p1.second;
	double b = p1.second * p2.first + p2.second * p3.first + p3.second * p1.first;
	return a - b;
}

bool compare1(const pair<double, double>& a, const pair<double, double>& b) {
	return (a.second == b.second ? a.first < b.first : a.second < b.second);
}

bool compare2(const pair<double, double>& a, const pair<double, double>& b) {
	double val = ccw(v[0], a, b);

	return (val ? val > 0 : compare1(a, b));
}

double dist(pair<double, double> a, pair<double, double> b) {
	return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	cin >> n >> l;

	for (int i = 0; i < n; i++) {
		double x, y;
		cin >> x >> y;
		v.push_back({ x, y });
	}

	sort(v.begin(), v.end(), compare1);
	res.push_back(v[0]);

	sort(v.begin() + 1, v.end(), compare2);
	res.push_back(v[1]);

	for (int i = 2; i < n; i++) {
		while (res.size() >= 2) {
			auto p1 = res.back();
			res.pop_back();
			auto p2 = res.back();

			auto val = ccw(p2, p1, v[i]);

			if (val > 0) {
				res.push_back(p1);
				break;
			}
		}

		res.push_back(v[i]);
	}

	res.push_back(res[0]);

	double ans = 4 * acos(0) * l;
	for (int i = 0; i < res.size() - 1; i++)
		ans += dist(res[i], res[i + 1]);

	cout << round(ans) << '\n';
	return 0;
}
