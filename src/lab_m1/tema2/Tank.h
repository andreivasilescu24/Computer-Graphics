#pragma once
#include "utils/glm_utils.h"

class Tank
{
public:
    float angle;
    int hp;

    float turretAngle;

    float projectileX, projectileY, projectileZ;
    float projectileAngle;
    float cooldown;

    glm::vec3 position;
    glm::vec3 forwardTank;
    glm::vec3 forwardTurret;
    
    Tank(glm::vec3 _position, float angle, float turret_angle, float projectile_x, float projectile_y, float projectile_z, float projectile_angle, int _hp, float _cooldown,
        const glm::vec3& _forwardTank, const glm::vec3& _forwardTurret)
        : position(_position),
          angle(angle),
          turretAngle(turret_angle),
            projectileX(projectile_x),
            projectileY(projectile_y),
            projectileZ(projectile_z),
            projectileAngle(projectile_angle),
            hp(_hp),
            cooldown(_cooldown),
            forwardTank(_forwardTank),
            forwardTurret(_forwardTurret)
    {
    }
};
