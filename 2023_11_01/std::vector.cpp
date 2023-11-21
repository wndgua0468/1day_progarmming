#include <iostream>
#include <vector>
#include <cmath>
#include <random>

double euclideanDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

int main() {
    // 난수 발생기 초기화
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    std::vector<int> A, B;

    // 0부터 100까지의 수를 랜덤으로 발생하여 처리
    for (int i = 0; i < 100; ++i) {
        int x = dis(gen);
        int y = dis(gen);

        // 유클리드 거리가 10 이내인 경우 A에 추가
        if (euclideanDistance(x, y, 30, 30) <= 10) {
            A.push_back(x);
        }

        //  유클리드 거리가 5 이내인 경우 B에 추가
        if (euclideanDistance(x, y, 70, 70) <= 5) {
            B.push_back(y);
        }
    }

    // 배열 A 출력
    std::cout << "A 배열: ";
    for (int num : A) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 배열 B 출력
    std::cout << "B 배열: ";
    for (int num : B) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
