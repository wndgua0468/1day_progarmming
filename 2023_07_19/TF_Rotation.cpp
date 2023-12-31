#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"

#define RAD2DEG(x)   x*180/M_PI  //라이안에더 디글
#define DEG2RAD(x)   x*M_PI/180  //디글에서 라디안

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


Pose2D    base_link_origin;      // 원점과 각도를 나타내는 변수
Point2D   base_link_Point2D, base_link_map_Point2D; // 변환할 포인트들

double angle_degree;
double angle_radian;


double Rotation_matrix[2][2];   //회전 행렬
double Rotation_matrix_inverse[2][2];   //역회전 행렬

void set_rotation_matrix_inverse(double m_angle_degree)
{
	angle_radian = DEG2RAD(m_angle_degree);

	Rotation_matrix_inverse[0][0] = cos(angle_radian);    Rotation_matrix_inverse[0][1] = sin(angle_radian);
	Rotation_matrix_inverse[1][0] = -sin(angle_radian);   Rotation_matrix_inverse[1][1] = cos(angle_radian);
}

void set_rotation_matrix(double m_angle_degree)  //각도를 라디안으로 변환
{
	angle_radian = DEG2RAD(m_angle_degree);

	Rotation_matrix[0][0] = cos(angle_radian);   Rotation_matrix[0][1] = -sin(angle_radian);
	Rotation_matrix[1][0] = sin(angle_radian);   Rotation_matrix[1][1] = cos(angle_radian);
}

// Base_Link_Map 좌표계에서 Base_Link 좌표계로 변환하는 함수
void TF_Change_base_link1(Point2D* base_link_map_point2d, Pose2D base_link_origin)  
{
	set_rotation_matrix_inverse(base_link_origin.theta);
	double transform_X = (Rotation_matrix_inverse[0][0] * base_link_map_point2d->x) + Rotation_matrix_inverse[0][1] * (base_link_map_point2d->y);
	double transform_Y = (Rotation_matrix_inverse[1][0] * base_link_map_point2d->x) + Rotation_matrix_inverse[1][1] * (base_link_map_point2d->y);

	base_link_map_point2d->x = transform_X;
	base_link_map_point2d->y = transform_Y;
}

// Base_Link 좌표계에서 Base_Link_Map 좌표계로 변환하는 함수
void TF_Change_base_link_map1(Point2D* base_link_point2d, Pose2D base_link_origin)  
{
	set_rotation_matrix(base_link_origin.theta);
	double transform_X = (Rotation_matrix[0][0] * base_link_point2d->x) + (Rotation_matrix[0][1] * base_link_point2d->y);
	double transform_Y = (Rotation_matrix[1][0] * base_link_point2d->x) + (Rotation_matrix[1][1] * base_link_point2d->y);
	base_link_point2d->x = transform_X;
	base_link_point2d->y = transform_Y;
}

// Base_Link_Map 좌표계에서 Base_Link 좌표계로 변환하는 함수
void TF_Change_base_link_map2(Point2D* base_link_point2d, Pose2D base_link_origin)  
{
	double transform_X = base_link_point2d->x + base_link_origin.x;
	double transform_Y = base_link_point2d->y + base_link_origin.y;
	base_link_point2d->x = transform_X;
	base_link_point2d->y = transform_Y;
}

// Base_Link 좌표계에서 Base_Link_Map 좌표계로 변환하는 함수
void TF_Change_base_link2(Point2D* base_link_map_point2d, Pose2D base_link_origin)  
{
	double transform_X = base_link_map_point2d->x - base_link_origin.x;
	double transform_Y = base_link_map_point2d->y - base_link_origin.y;
	base_link_map_point2d->x = transform_X;
	base_link_map_point2d->y = transform_Y;
}

// Base_Link_Map 좌표계에서 Base_Link 좌표계로 변환 후 다시 Base_Link_Map 좌표계로 변환하는 함수
void TF_base_link_map_map(Point2D* base_link_point2d, Point2D* base_link_map_point2d, Pose2D base_link_origin)  
{
	TF_Change_base_link2(base_link_map_point2d, base_link_origin);  // Base_Link_Map 좌표계로 변환
	TF_Change_base_link1(base_link_map_point2d, base_link_origin);  // Base_Link 좌표계로 변환
}

// Base_Link 좌표계에서 Base_Link_Map 좌표계로 변환 후 다시 Base_Link 좌표계로 변환하는 함수
void TF_map_base_link_map(Point2D* base_link_point2d, Point2D* base_link_map_point2d, Pose2D base_link_origin)  
{ 
	TF_Change_base_link1(base_link_point2d, base_link_origin);  // Base_Link 좌표계로 변환
	TF_Change_base_link2(base_link_point2d, base_link_origin);  // Base_Link_Map 좌표계로 변환
}

int main(void)
{

	base_link_origin.x = 2.0;
	base_link_origin.y = 1.0;
	base_link_origin.theta = -90;

	base_link_Point2D.x = 1.0;
	base_link_Point2D.y = 1.0;

	base_link_map_Point2D.x = 1.0;
	base_link_map_Point2D.y = -1.0;

	set_rotation_matrix(base_link_origin.theta);

	// Base_Link 좌표계에서 Base_Link_Map 좌표계로 변환 후 다시 Base_Link_Map 좌표계로 변환하는 함수 사용
	TF_base_link_map_map(&base_link_Point2D, &base_link_map_Point2D, base_link_origin);
	printf("Transformed Point Matrix (base_link to map):\n");
	printf("Transformed Point: %6.3lf  %6.3lf\n", base_link_map_Point2D.x, base_link_map_Point2D.y);

	// Base_Link_Map 좌표계에서 Base_Link 좌표계로 변환 후 다시 Base_Link 좌표계로 변환하는 함수 사용
	TF_map_base_link_map(&base_link_map_Point2D, &base_link_Point2D, base_link_origin);
	printf("Transformed Point Matrix (map to base_link):\n");
	printf("Transformed Point: %6.3lf  %6.3lf\n", base_link_Point2D.x, base_link_Point2D.y);

	return 0;
}
