#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
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

void TF_base_link_base_link_map(Point2D* base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    double Rotation_matrix_inverse[2][2];
    set_rotation_matrix(-base_link_origin.theta, Rotation_matrix_inverse);

    double transformed_x = (Rotation_matrix_inverse[0][0] * (base_link_Point2D->x - base_link_origin.x)) +
        (Rotation_matrix_inverse[0][1] * (base_link_Point2D->y - base_link_origin.y));
    double transformed_y = (Rotation_matrix_inverse[1][0] * (base_link_Point2D->x - base_link_origin.x)) +
        (Rotation_matrix_inverse[1][1] * (base_link_Point2D->y - base_link_origin.y));

    base_link_map_point2d->x = transformed_x;
    base_link_map_point2d->y = transformed_y;
}

void TF_base_link_map_base_link(Point2D* base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    double Rotation_matrix[2][2];
    set_rotation_matrix(base_link_origin.theta, Rotation_matrix);

    double transformed_x_inverse = (Rotation_matrix[0][0] * base_link_Point2D->x) +
        (Rotation_matrix[0][1] * base_link_Point2D->y) + base_link_origin.x;
    double transformed_y_inverse = (Rotation_matrix[1][0] * base_link_Point2D->x) +
        (Rotation_matrix[1][1] * base_link_Point2D->y) + base_link_origin.y;

    base_link_map_point2d->x = transformed_x_inverse;
    base_link_map_point2d->y = transformed_y_inverse;
}

int main(void)
{
    // 구조체 변수를 선언하고 초기화
    Pose2D base_link_origin = { 1.0, 2.0, -90.0 };
    Point2D base_link_Point2D = { 1.0, 1.0 };
    Point2D base_link_map_Point2D = { -1.0, 1.0 };

    // 회전 변환 행렬과 역변환 행렬을 설정
    double Rotation_matrix[2][2];
    set_rotation_matrix(base_link_origin.theta, Rotation_matrix);

    double Rotation_matrix_inverse[2][2];
    set_rotation_matrix(-base_link_origin.theta, Rotation_matrix_inverse);

    printf("Rotation Matrix:\n");
    printf("%6.3lf  %6.3lf\n", Rotation_matrix[0][0], Rotation_matrix[0][1]);
    printf("%6.3lf  %6.3lf\n", Rotation_matrix[1][0], Rotation_matrix[1][1]);

    printf("Rotation Matrix_inverse:\n");
    printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[0][0], Rotation_matrix_inverse[0][1]);
    printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[1][0], Rotation_matrix_inverse[1][1]);

    // 변환을 수행하고 결과를 출력
    Point2D transformed_point;
    TF_base_link_base_link_map(&base_link_Point2D, &transformed_point, base_link_origin);
    printf("Transformed Point Matrix:\n");
    printf("Transformed Point: %6.3lf  %6.3lf\n", transformed_point.x, transformed_point.y);

    Point2D transformed_point_inverse;
    TF_base_link_map_base_link(&base_link_map_Point2D, &transformed_point_inverse, base_link_origin);
    printf("Transformed Point Matrix_inverse:\n");
    printf("Transformed Point_Inverse: %6.3lf  %6.3lf\n", transformed_point_inverse.x, transformed_point_inverse.y);

    return 0;
}



