#include "stdafx.h"
#define _CRT_SECURE_NO_WARNING
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>

typedef struct
{
    double x;
    double y;
    double theta;
} Pose2D;

typedef struct
{
    double x;
    double y;
} Point2D;

double solve_x(double angle_degree)
{
    double x = cos(angle_degree) - sin(angle_degree);
    return x;
}

double solve_y(double angle_degree)
{
    double y = sin(angle_degree) + cos(angle_degree);
    return y;
}

double DEG2RAD(double x)
{
    return x * M_PI / 180.0;
}

double RAD2DEG(double x)
{
    return x * 180.0 / M_PI;
}

void set_rotation_matrix(double m_angle_degree, double Rotation_matrix[2][2])
{
    double angle_radian = DEG2RAD(m_angle_degree);
    Rotation_matrix[0][0] = cos(angle_radian); Rotation_matrix[0][1] = -sin(angle_radian);
    Rotation_matrix[1][0] = sin(angle_radian); Rotation_matrix[1][1] = cos(angle_radian);
}

void TF_base_link_base_link_map(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    double Rotation_matrix_inverse[2][2];
    set_rotation_matrix(-base_link_origin.theta, Rotation_matrix_inverse);

    double transformed_x = (Rotation_matrix_inverse[0][0] * (base_link_Point2D.x - base_link_origin.x)) +
        (Rotation_matrix_inverse[0][1] * (base_link_Point2D.y - base_link_origin.y));
    double transformed_y = (Rotation_matrix_inverse[1][0] * (base_link_Point2D.x - base_link_origin.x)) +
        (Rotation_matrix_inverse[1][1] * (base_link_Point2D.y - base_link_origin.y));

    base_link_map_point2d->x = transformed_x;
    base_link_map_point2d->y = transformed_y;
}

void TF_base_link_map_base_link(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    double Rotation_matrix[2][2];
    set_rotation_matrix(base_link_origin.theta, Rotation_matrix);

    double transformed_x_inverse = (Rotation_matrix[0][0] * base_link_Point2D.x) +
        (Rotation_matrix[0][1] * base_link_Point2D.y) + base_link_origin.x;
    double transformed_y_inverse = (Rotation_matrix[1][0] * base_link_Point2D.x) +
        (Rotation_matrix[1][1] * base_link_Point2D.y) + base_link_origin.y;

    base_link_map_point2d->x = transformed_x_inverse;
    base_link_map_point2d->y = transformed_y_inverse;
}

int main(void)
{
    double angle;
    double angle_degree;
    double x_new1;
    double y_new1;
    double x_new2;
    double y_new2;
    double answer1_x = 0;
    double answer1_y = 0;
    double answer2_x = 0;
    double answer2_y = 0;

    // 기존 코드의 Pose2D와 Point2D를 사용하여 초기화
    Pose2D base_link_origin = { 1.0, 2.0, -90.0 };
    Point2D base_link_Point2D = { 1.0, 1.0 };
    Point2D base_link_map_Point2D = { -1.0, 1.0 };

    printf("Base_link의 angle값을 입력하세요: ");
    scanf_s("%lf", &angle);
    printf("Base_link의 X값을 입력하세요: ");
    scanf_s("%lf", &x_new1);
    printf("Base_link의 Y값을 입력하세요: ");
    scanf_s("%lf", &y_new1);

    angle_degree = angle * 3.141592 / 180;
    answer1_x = x_new1 + solve_x(angle_degree);
    answer1_y = y_new1 + solve_y(angle_degree);

    printf("병진운동하기 전 X값을 입력하세요: ");
    scanf_s("%lf", &x_new2);
    printf("병진운동하기 전 Y값을 입력하세요: ");
    scanf_s("%lf", &y_new2);

    answer2_x = answer1_x + x_new2;
    answer2_y = answer1_y + y_new2;

    printf("Xmap: %lf, Ymap: %lf\n", answer2_x, answer2_y);

    // 기존의 변환 함수를 사용하여 통합
    Point2D transformed_point;
    TF_base_link_base_link_map(base_link_Point2D, &transformed_point, base_link_origin);
    printf("Transformed Point Matrix:\n");
    printf("Transformed Point: %6.3lf  %6.3lf\n", transformed_point.x, transformed_point.y);

    Point2D transformed_point_inverse;
    TF_base_link_map_base_link(base_link_map_Point2D, &transformed_point_inverse, base_link_origin);
    printf("Transformed Point Matrix_inverse:\n");
    printf("Transformed Point_Inverse: %6.3lf  %6.3lf\n", transformed_point_inverse.x, transformed_point_inverse.y);

    return 0;
}
