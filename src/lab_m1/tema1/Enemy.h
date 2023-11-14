#pragma once
#include <string>

class Enemy
{
public:
    std::string color;
    float X, Y;
    bool isInRedZone;
    int lineIndex, hits;
    float scaleX, scaleY;

    Enemy(const std::string& color, float x, float y, int line_index, bool is_in_red_zone = false, int hits = 0, float scale_x = 1, float scale_y = 1)
        : color(color),
          X(x),
          Y(y),
        isInRedZone(is_in_red_zone),
        lineIndex(line_index),
        hits(hits),
        scaleX(scale_x),
        scaleY(scale_y)
    {
    }
};
