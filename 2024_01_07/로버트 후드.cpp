#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#define INF 2e9
#define LL long long
#define FASTIO cin.tie(0); cout.tie(0); ios::sync_with_stdio(false);

using namespace std;
struct Point {
    LL X, Y;
};

int T;
LL N, X, Y, minX, maxY, First, Left, Right, CV_Count;
vector<Point> Vec;
vector<Point> CV;
int Answer;

void init() {
    minX = INF;
    maxY = -INF;
}

void input() {
    cin >> N;
    for (int i = 0; i < N; i++) {
        cin >> X >> Y;
        Vec.push_back({ X, Y });
    }
}

LL CCW(const Point& A, const Point& B, const Point& C) {
    LL OP = A.X * B.Y + B.X * C.Y + C.X * A.Y;
    OP -= (A.Y * B.X + B.Y * C.X + C.Y * A.X);
    return OP;
}

bool Comp(const Point& A, const Point& B) {
    LL ccwValue = CCW(Vec[0], A, B);
    if (ccwValue < 0) {
        return true;
    }
    else if (ccwValue > 0) {
        return false;
    }
    if (A.Y != B.Y) {
        return (A.Y > B.Y);
    }
    return (A.X < B.X);
}

void settings() {
    for (int i = 0; i < Vec.size(); i++) {
        if ((maxY < Vec[i].Y) || ((maxY == Vec[i].Y) && (minX > Vec[i].X))) {
            minX = Vec[i].X;
            maxY = Vec[i].Y;
            First = i;
        }
    }
    Vec[First].X = Vec[0].X;
    Vec[First].Y = Vec[0].Y;
    Vec[0].X = minX;
    Vec[0].Y = maxY;
    sort(Vec.begin() + 1, Vec.end(), Comp);
    for (int i = 0; i < Vec.size(); i++) {
        while ((CV.size() >= 2) && (CCW(CV[CV.size() - 2], CV.back(), Vec[i]) >= 0)) {
            CV.pop_back();
        };
        CV.push_back(Vec[i]);
    }
    CV_Count = CV.size();
    for (int i = 0; i < CV_Count; i++) {
        if (CV[i].X < CV[Left].X) {
            Left = i;
        }
        if (CV[i].X > CV[Right].X) {
            Right = i;
        }
    }
    Answer = (CV[Left].X - CV[Right].X) * (CV[Left].X - CV[Right].X) + (CV[Left].Y - CV[Right].Y) * (CV[Left].Y - CV[Right].Y);
    Point Center;
    Center.X = 0;
    Center.Y = 0;
    for (int i = 0; i < CV_Count; i++) {
        Point Prev;
        Prev.X = CV[(Left + 1) % CV_Count].X - CV[Left].X;
        Prev.Y = CV[(Left + 1) % CV_Count].Y - CV[Left].Y;
        Point Next;
        Next.X = CV[Right].X - CV[(Right + 1) % CV_Count].X;
        Next.Y = CV[Right].Y - CV[(Right + 1) % CV_Count].Y;
        if (CCW(Center, Prev, Next) < 0) {
            Left = (Left + 1) % CV_Count;
        }
        else {
            Right = (Right + 1) % CV_Count;
        }
        int Value = (CV[Left].X - CV[Right].X) * (CV[Left].X - CV[Right].X) + (CV[Left].Y - CV[Right].Y) * (CV[Left].Y - CV[Right].Y);
        Answer = max(Answer, Value);
    }
}

void find_Answer() {
    cout << fixed;
    cout.precision(8);
    cout << sqrt(Answer) << "\n";
}

int main() {
    FASTIO
        init();
    input();
    settings();
    find_Answer();

    return 0;
}
