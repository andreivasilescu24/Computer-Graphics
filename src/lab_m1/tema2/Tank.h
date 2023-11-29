#pragma once

class Tank
{
public:
    float x, y, z;
    float angle;
    bool forward;

    float turretAngle;
    
    Tank(float x, float y, float z, float angle, float turret_angle, bool forward)
        : x(x),
          y(y),
          z(z),
          angle(angle),
          turretAngle(turret_angle),
          forward(forward)
    {
    }
};
