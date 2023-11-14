#pragma once
#include <string>

class ShootingStar
{
public:
    int X, Y;
    float angularStep;
    std::string color;
    int outerRadius;
    int innerRadius;
    int matrixSquareI, matrixSquareJ;

    ShootingStar(int x, int y, float angular_step, const std::string& color, int matrix_squareI, int matrix_squareJ, int outer_radius = 30, int inner_radius = 10)
        : X(x),
          Y(y),
          angularStep(angular_step),
          color(color),
        matrixSquareI(matrix_squareI),
        matrixSquareJ(matrix_squareJ),
          outerRadius(outer_radius),
          innerRadius(inner_radius)    {
    }
};
