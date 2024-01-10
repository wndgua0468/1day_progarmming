#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

class ElectricCircuit {
private:
    const float L = 0.6;
    const float Vmax = 150.0;
    const float f = 60.0;

    float R, C;
    float XL, XC, Z, Imax, theta, VRmax, VLmax, VCmax;

public:
    void getInput() {
        std::cout << "저항 R을 입력하세요 (Ohm): ";
        std::cin >> R;
        std::cout << "축전기 전기용량 C를 입력하세요 (Farad): ";
        std::cin >> C;
    }

    void calculateValues() {
        XL = 2 * M_PI * f * L;
        XC = 1 / (2 * M_PI * f * C);
        Z = sqrt(R * R + (XL - XC) * (XL - XC));
        Imax = Vmax / Z;
        theta = atan((XL - XC) / R) * (180.0 / M_PI);
        VRmax = Imax * R;
        VLmax = Imax * XL;
        VCmax = Imax * XC;
    }

    void displayResults() {
        std::cout << "유도 리액턴스 (XL): " << XL << " Ohm\n";
        std::cout << "용량 리액턴스 (XC): " << XC << " Ohm\n";
        std::cout << "최대 전류 (Imax): " << Imax << " A\n";
        std::cout << "임피던스 (Z): " << Z << " Ohm\n";
        std::cout << "위상각 (theta): " << theta << " degree\n";
        std::cout << "VRmax: " << VRmax << " Volt\n";
        std::cout << "VLmax: " << VLmax << " Volt\n";
        std::cout << "VCmax: " << VCmax << " Volt\n";
    }
};

int main() {
    ElectricCircuit circuit;
    circuit.getInput();
    circuit.calculateValues();
    circuit.displayResults();

    return 0;
}
