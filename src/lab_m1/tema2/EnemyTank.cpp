#include "EnemyTank.h"

void EnemyTank::updateTimerMove(float deltaTime)
{
    timerMove += deltaTime;
}

void EnemyTank::updateMovementState(int index)
{
    if(movementState == "FORWARD" || movementState == "BACKWARD")
    {
        if(index == 0)
        {
            movementState = "LEFT";
        } else
        {
            movementState = "RIGHT";
        }
    } else
    {
        if(index == 0)
        {
            movementState = "FORWARD";
        } else
        {
            movementState = "BACKWARD";
        }
    }
}