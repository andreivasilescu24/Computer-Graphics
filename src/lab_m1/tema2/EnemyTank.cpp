#include "EnemyTank.h"

#include "Tema2.h"

void EnemyTank::updateTimerMove(float deltaTime)
{
    timerMove += deltaTime;
}

void EnemyTank::updateMovementState(std::string new_state, float new_target_seconds)
{
    movementState = new_state;
    targetSecondsTank = new_target_seconds;
    timerMove = 0;
}

void EnemyTank::updateTankPosition(float deltaTime)
{
    if(movementState == "FORWARD")
    {
        position += forwardTank * deltaTime * 2.0f;
    } else if(movementState == "BACKWARD")
    {
        position -= forwardTank * deltaTime * 2.0f;
    } else if(movementState == "LEFT")
    {
        angle += deltaTime * 100;
        forwardTank = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(forwardTank, 1)));
    } else if(movementState == "RIGHT")
    {
        angle -= deltaTime * 100;
        forwardTank = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(forwardTank, 1)));
    }
}
