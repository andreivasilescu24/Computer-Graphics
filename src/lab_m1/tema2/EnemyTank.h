#pragma once
#include <vector>

#include "Projectile.h"
#include "utils/glm_utils.h"

class EnemyTank
{
public:
    float angle;
    int hp;

    float turretAngle;
    
    float cooldown;

    glm::vec3 position;
    glm::vec3 forwardTank;
    glm::vec3 forwardTurret;

    std::vector<Projectile> projectiles;
    bool shoot;

    std::string movementState;
    float timerMove;
    float targetSeconds;
    
    EnemyTank(float angle, int hp, float turret_angle, float cooldown, const glm::vec3& position, const glm::vec3& forward_tank,
        const glm::vec3& forward_turret, std::string _movementState, float _targetSeconds, bool _shoot = false, float _timerMove = 0)
        : angle(angle),
          hp(hp),
          turretAngle(turret_angle),
          cooldown(cooldown),
          position(position),
          forwardTank(forward_tank),
          forwardTurret(forward_turret),
          shoot(_shoot),
            movementState(_movementState),
            targetSeconds(_targetSeconds),
            timerMove(_timerMove)
    {
    }

    void updateTimerMove(float deltaTime);
    void updateMovementState(int index);
};
