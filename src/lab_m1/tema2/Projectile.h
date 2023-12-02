#pragma once
#include <glm/vec3.hpp>
#include <string>

class Projectile
{
public:
    bool hit;
    glm::vec3 position, forward;
    float angle;
    float time;

    Projectile(bool hit, glm::vec3 _position, glm::vec3 _forward, float _angle, float _time = 0)
        : hit(hit),
          position(_position),
          forward(_forward),
            angle(_angle),
            time(_time)
    {
    }
};
