#pragma once
#include <vector>

#include "Projectile.h"
#include "utils/glm_utils.h"
#include "Tank.h"

class EnemyTank
{
public:
    float angle;
    float hp;

    float turretAngle;
    float cooldown;

    glm::vec3 position;
    glm::vec3 forwardTank;
    glm::vec3 forwardTurret;

    std::vector<Projectile> projectiles;
    bool shoot;

    std::string movementState;
    std::string turretState;
    float timerMove;
    float timerTurretMove;
    float targetSecondsTank;
    float targetSecondsTurret;

    EnemyTank* collisionTank = NULL;
    
    EnemyTank(float angle, float _hp, float turret_angle, float cooldown, const glm::vec3& position, const glm::vec3& forward_tank,
        const glm::vec3& forward_turret, std::string _movementState, std::string _turretState, float _targetSecondsTank, float _targetSecondsTurret,
        bool _shoot = false, float _timerMove = 0, float _timerTurretMove = 0)
        : angle(angle),
          hp(_hp),
          turretAngle(turret_angle),
          cooldown(cooldown),
          position(position),
          forwardTank(forward_tank),
          forwardTurret(forward_turret),
          shoot(_shoot),
            movementState(_movementState),
            turretState(_turretState),
            targetSecondsTank(_targetSecondsTank),
            targetSecondsTurret(_targetSecondsTurret),
            timerMove(_timerMove),
            timerTurretMove(_timerTurretMove)
    {
    }

    void updateTimerMove(float deltaTime);
    void updateMovementState(std::string new_state, float new_target_seconds);
    void updateTimerTurretMove(float deltaTime);
    void updateTankPosition(float deltaTime);
    void updateTurretPosition();
    bool checkTankNearby(Tank playerTank, int tankRadius);
};
