#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;

struct Point {
    long long x;
    long long y;

    Point(long long x, long long y) : x(x), y(y) {}
};

Point root(LLONG_MAX, LLONG_MAX);

int ccw(Point p1, Point p2, Point p3) {
    long long result = (p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p2.x * p1.y + p3.x * p2.y + p1.x * p3.y);

    if (result > 0) {   // 반시계 방향
        return 1;
    }
    else if (result < 0) {    // 시계 방향
        return -1;
    }
    else {
        return 0;
    }
}

long long dist(Point p1, Point p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

int grahamScan(vector<Point>& input) {
    // 기준점 찾기
    for (int i = 0; i < input.size(); i++) {
        if (input[i].y < root.y || (input[i].y == root.y && input[i].x < root.x)) {
            root = input[i];
        }
    }

    // 모든 점들을 반시계 방향으로 정렬하기
    sort(input.begin(), input.end(), [&](Point p1, Point p2) {
        int result = ccw(root, p1, p2);

        if (result > 0) {
            return true;
        }
        else if (result < 0) {
            return false;
        }
        else {
            long long distance1 = dist(root, p1);
            long long distance2 = dist(root, p2);

            return distance1 < distance2;
        }
        });

    stack<Point> stack;
    stack.push(root);

    for (int i = 1; i < input.size(); i++) {
        while (stack.size() > 1 && ccw(stack.top(), input[i], stack.top()) <= 0) {    // first, second, next
            stack.pop();    // second 빼기
        }
        stack.push(input[i]);    // next 넣기
    }

    return stack.size();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    vector<Point> points;

    for (int i = 0; i < N; i++) {
        long long x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }

    cout << grahamScan(points) << "\n";

    return 0;
}
