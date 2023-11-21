#include <stdio.h>

#define NO_CALIBRATION_POINT 5

struct CalibrationData {
    double X[NO_CALIBRATION_POINT];
    double y[NO_CALIBRATION_POINT];
} cal_data;

double linear_mapping(double x, struct CalibrationData cal_data)
{
    double y = 0.0;

    int i = 0;
    for (i = 0; i < NO_CALIBRATION_POINT - 1; i++) {
        if (x >= cal_data.X[i] && x < cal_data.X[i + 1]) {
            double x1 = cal_data.X[i];
            double x2 = cal_data.X[i + 1];
            double y1 = cal_data.y[i];
            double y2 = cal_data.y[i + 1];
          
            y = y1 + (x - x1) * ((y2 - y1) / (x2 - x1));
            break;
        }
    }
    if (i == NO_CALIBRATION_POINT - 1) {
        y = cal_data.y[i]; // 마지막 보정값 사용
    }
    return y;
}
int main()
{
    cal_data = {
        {1.0, 2.0, 3.0, 4.0, 5.0},
        {0.0, 10.0, 20.0, 30.0, 40.0}
    };
    double x = 2.0;
    double Value = linear_mapping(x, cal_data);
    printf("Value: %f\n", Value);

    return 0;
}
