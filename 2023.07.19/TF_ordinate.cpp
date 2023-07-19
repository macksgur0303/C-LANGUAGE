﻿#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAD2DEG(x) (x * 180 / M_PI)
#define DEG2RAD(x) (x / 180 * M_PI)

typedef struct {
    double x;
    double y;
    double theta;
} Pose2D;

typedef struct {
    double x;
    double y;
} Point2D;

Pose2D    base_link_origin;
Point2D   base_link_Point2D, base_link_Point, base_link_map_Point2D, base_link_map_Point;

double angle_degree;
double angle_radian;
double Rotation_matrix[2][2];
double Rotation_matrix_inverse[2][2];

void set_rotation_matrix_inverse(double m_angle_degree)
{
    angle_radian = DEG2RAD(m_angle_degree);
    Rotation_matrix_inverse[0][0] = cos(angle_radian);    Rotation_matrix_inverse[0][1] = sin(angle_radian);
    Rotation_matrix_inverse[1][0] = -sin(angle_radian);   Rotation_matrix_inverse[1][1] = cos(angle_radian);
    //printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[0][0], Rotation_matrix_inverse[0][1]);
    //printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[1][0], Rotation_matrix_inverse[1][1]);
}

void set_rotation_matrix(double m_angle_degree)
{
    angle_radian = DEG2RAD(m_angle_degree);
    Rotation_matrix[0][0] = cos(angle_radian);    Rotation_matrix[0][1] = -sin(angle_radian);
    Rotation_matrix[1][0] = sin(angle_radian);    Rotation_matrix[1][1] = cos(angle_radian);
    //printf("%6.3lf  %6.3lf\n", Rotation_matrix[0][0], Rotation_matrix[0][1]);
    //printf("%6.3lf  %6.3lf\n", Rotation_matrix[1][0], Rotation_matrix[1][1]);
}

void TF_base_link_base_link_map(Point2D base_link_point2d, Point2D* base_link_map_point2d, Pose2D base_link_origin) {

    base_link_map_Point.x = base_link_point2d.x * Rotation_matrix[0][0] + base_link_point2d.y * Rotation_matrix[0][1];
    base_link_map_Point.y = base_link_point2d.x * Rotation_matrix[1][0] + base_link_point2d.y * Rotation_matrix[1][1];

    base_link_map_point2d->x = base_link_map_Point.x;
    base_link_map_point2d->y = base_link_map_Point.y;

}

void TF_base_link_map_base_link(Point2D base_link_point2d, Point2D* base_link_map_point2d, Pose2D base_link_origin) {
    base_link_point2d.x = base_link_map_point2d->x * Rotation_matrix_inverse[0][0] + base_link_map_point2d->y * Rotation_matrix_inverse[0][1];
    base_link_point2d.y = base_link_map_point2d->x * Rotation_matrix_inverse[1][0] + base_link_map_point2d->y * Rotation_matrix_inverse[1][1];
    base_link_point2d.x = base_link_point2d.x;
    base_link_point2d.y = base_link_point2d.y;
    printf("\n\n");
}

void TF_map_base_link(Point2D base_link_2d, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    base_link_map_point2d->x -= base_link_origin.x;
    base_link_map_point2d->y -= base_link_origin.y;
    set_rotation_matrix_inverse(base_link_origin.theta);
    TF_base_link_map_base_link(base_link_2d, base_link_map_point2d, base_link_origin);

    printf("\n\n");
    printf("%6.3lf  %6.3lf\n", base_link_2d.x, base_link_2d.y);


}

void TF_base_link_map(Point2D base_link_2d, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
    set_rotation_matrix(base_link_origin.theta);
    TF_base_link_base_link_map(base_link_Point2D, &base_link_map_Point2D, base_link_origin);

    base_link_origin.x = base_link_map_point2d->x + base_link_origin.x;
    base_link_origin.y = base_link_map_point2d->y + base_link_origin.y;
    printf("\n\n");
    printf("%6.3lf  %6.3lf\n", base_link_origin.x, base_link_origin.y);
}

int main() {
    base_link_origin.x = 0.0;
    base_link_origin.y = 0.0;
    base_link_origin.theta = 90;

    base_link_Point2D.x = 4;
    base_link_Point2D.y = 2;

    base_link_map_Point2D.x = -2;
    base_link_map_Point2D.y = 4;

    TF_base_link_map(base_link_Point2D, &base_link_map_Point2D, base_link_origin);

    printf("\n");

    base_link_origin.x = 0.0;
    base_link_origin.y = 0.0;
    base_link_origin.theta = 90;

    base_link_Point2D.x = 4;
    base_link_Point2D.y = 2;

    base_link_map_Point2D.x = -2;
    base_link_map_Point2D.y = 4;

    TF_map_base_link(base_link_Point2D, &base_link_map_Point2D, base_link_origin);


    return 0;
}