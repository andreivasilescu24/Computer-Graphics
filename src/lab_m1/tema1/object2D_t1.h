#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_t1
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateGun(const std::string &name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill);
    Mesh* CreateEnemy(const std::string &name, glm::vec3 center, float lengthBigHexa, float lengthSmallHexa, glm::vec3 colorBigHexa, glm::vec3 colorSmallHexa, bool fill = false);
    Mesh* CreateStar(const std::string &name, glm::vec3 centerStar, glm::vec3 color, float outerRadius, float innerRadius, bool fill = false);
}
