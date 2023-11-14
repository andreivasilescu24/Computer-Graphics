#pragma once

class GunSquare
{
public:
    int index;
    float leftX, rightX, upY, downY;

    GunSquare(int _index, float left_x, float right_x, float up_y, float down_y)
        :   index(_index),
            leftX(left_x),
            rightX(right_x),
            upY(up_y),
            downY(down_y)
    {
    }
};
