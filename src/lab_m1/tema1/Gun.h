#pragma once
#include <string>

class Gun
{
public:
    std::string color, name;
    float X, Y;
    float scaleX, scaleY;
    int matrixSquareI, matrixSquareJ, cost;
    bool shoot;
    float timeBetweenStars;
    bool isHit;

    Gun(const std::string& _color, const std::string& name, float x, float y, float scale_x, float scale_y, int matrix_square_i, int matrix_square_j,
        int _cost, bool _shoot = false, float _timeBetweenStars = 0, bool _isHit = false)
        :   color(_color),
        name(name),
        X(x),
        Y(y),
        scaleX(scale_x),
        scaleY(scale_y),
        matrixSquareI(matrix_square_i),
        matrixSquareJ(matrix_square_j),
        cost(_cost),
        shoot(_shoot),
        timeBetweenStars(_timeBetweenStars),
        isHit(_isHit)
    {
    }
};
