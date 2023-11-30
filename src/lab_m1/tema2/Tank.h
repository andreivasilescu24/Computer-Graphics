#pragma once

class Tank
{
public:
    float x, y, z;
    float angle;
    bool forward;
    glm::vec3 currPosition;
    glm::vec3 directionVector;

    float turretAngle;

    float projectileX, projectileY, projectileZ;
    float projectileAngle;
    
    Tank(float x, float y, float z, float angle, float turret_angle, bool forward, float projectile_x, float projectile_y, float projectile_z, float projectile_angle)
        : x(x),
          y(y),
          z(z),
          angle(angle),
          turretAngle(turret_angle),
          forward(forward),
            projectileX(projectile_x),
            projectileY(projectile_y),
            projectileZ(projectile_z),
            projectileAngle(projectile_angle)
    {
    }
};
