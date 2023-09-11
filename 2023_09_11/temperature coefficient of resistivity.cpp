#define _CRT_SECURE_NO_WARNINGS﻿
#define temperature_coefficient_of_resistivity_Point 7

#include <stdio.h>
#include <math.h>
struct
{

    double X[temperature_coefficient_of_resistivity_Point];
    double Y[temperature_coefficient_of_resistivity_Point];

}cal_data;

double linear_mapping(double y)  // AD 변환값을 넣으면 steering 값이 나옴
{

    int i, j;
    double x;

    for (j = 0; j < temperature_coefficient_of_resistivity_Point - 1; j++)
    {

        if (y < cal_data.Y[0])
            i = 0;

        else if ((y >= cal_data.Y[j]) && (y < cal_data.Y[j + 1]))
            i = j;

        else
            i = temperature_coefficient_of_resistivity_Point - 2;
    }

    double x1 = cal_data.X[i];
    double x2 = cal_data.X[i + 1];
    double y1 = cal_data.Y[i];
    double y2 = cal_data.Y[i + 1];

    double R0 = 1.0;

    x = (y - R0) / ((y2 - y1) / (x2 - x1));

    return x;
}

int main()
{

    cal_data =
    {
        {106.0, 315.0, 481.0, 736.0, 936.0, 1083.0, 1232.0},
        {1.56, 2.67 , 3.55, 4.90, 5.95, 6.74, 7.53}
    };

    double y = 3.0 / (340.0 / pow(10.0, 3.0));

    printf("%lf", linear_mapping(y));
}
