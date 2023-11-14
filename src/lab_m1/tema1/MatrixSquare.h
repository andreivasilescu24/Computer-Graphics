#pragma once

class MatrixSquare
{
public:
    float isFull;
    float leftX, rightX, upY, downY;
    bool emptySquare;

    MatrixSquare(float is_full, float left_x, float right_x, float up_y, float down_y, bool empty_square = false)
        : isFull(is_full),
          leftX(left_x),
          rightX(right_x),
          upY(up_y),
          downY(down_y),
            emptySquare(empty_square)
    {
    }
};
