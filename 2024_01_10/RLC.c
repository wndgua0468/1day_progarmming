#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

int main() {
    const float L = 0.6;
    const float Vmax = 150.0;
    const float f = 60.0;

    float R, C;
    float XL, XC, Z, Imax, theta, VRmax, VLmax, VCmax;

    printf("저항 R을 입력하세요: (Ohm): ");
    scanf("%f", &R);
    printf("축전기 전기용량 C를 입력하세요: (Farad): ");
    scanf("%f", &C);

    XL = 2 * M_PI * f * L;
    XC = 1 / (2 * M_PI * f * C);
    Z = sqrt(R * R + (XL - XC) * (XL - XC));
    Imax = Vmax / Z;
    theta = atan((XL - XC) / R) * (180.0 / M_PI);
    VRmax = Imax * R;
    VLmax = Imax * XL;
    VCmax = Imax * XC;

    printf("유도 리액턴스 (XL): %.2f Ohm\n", XL);
    printf("용량 리액턴스 (XC): %.2f Ohm\n", XC);
    printf("최대 전류 (Imax): %.2f A\n", Imax);
    printf("임피던스 (Z): %.2f Ohm\n", Z);
    printf("위상각 (theta): %.2f degree\n", theta);
    printf("VRmax: %.2f Volt\n", VRmax);
    printf("VLmax: %.2f Volt\n", VLmax);
    printf("VCmax: %.2f Volt\n", VCmax);

    return 0;
}
