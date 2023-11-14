#include "object2D_t1.h"

#include <iostream>
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D_t1::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D_t1::CreateGun(const std::string &name,
    glm::vec3 center,
    float height,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 centerGun = center;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(centerGun, color),
        VertexFormat(centerGun + glm::vec3(width / 2, 0, 0), color),
        VertexFormat(centerGun + glm::vec3(0, height / 2, 0), color),
        VertexFormat(centerGun + glm::vec3(-width / 2, 0, 0), color),
        VertexFormat(centerGun + glm::vec3(0, -height / 2, 0), color),
        VertexFormat(centerGun + glm::vec3(width / 3, height / 5.9f, 0), color),
        VertexFormat(centerGun + glm::vec3(width / 3, -height / 5.9f, 0), color),
        VertexFormat(centerGun + glm::vec3(width / 1.4f, height / 6, 0), color),
        VertexFormat(centerGun + glm::vec3(width / 1.4f, -height / 6, 0), color)
    };

    Mesh* gun = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 6, 1, 8, 8, 1, 7, 7, 1, 5, 5, 1, 6};

    if (!fill) {
        gun->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    gun->InitFromData(vertices, indices);
    return gun;
}

Mesh* object2D_t1::CreateEnemy(
    const std::string &name,
    glm::vec3 centerHexa,
    float lengthBigHexa,
    float lengthSmallHexa,
    glm::vec3 colorBigHexa,
    glm::vec3 colorSmallHexa,
    bool fill)
{
    glm::vec3 center = centerHexa;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center, colorSmallHexa),
        VertexFormat(center + glm::vec3(lengthBigHexa, 0, -1), colorBigHexa),
        VertexFormat(center + glm::vec3(lengthBigHexa / 2, lengthBigHexa * sqrt(3) / 2, -1), colorBigHexa),
        VertexFormat(center + glm::vec3(-lengthBigHexa / 2, lengthBigHexa * sqrt(3) / 2, -1), colorBigHexa),
        VertexFormat(center + glm::vec3(-lengthBigHexa, 0, 0), colorBigHexa),
        VertexFormat(center + glm::vec3(-lengthBigHexa / 2, -lengthBigHexa * sqrt(3) / 2, -1), colorBigHexa),
        VertexFormat(center + glm::vec3(lengthBigHexa / 2, -lengthBigHexa * sqrt(3) / 2, -1), colorBigHexa),
        VertexFormat(center + glm::vec3(lengthSmallHexa, 0, 0), colorSmallHexa),
        VertexFormat(center + glm::vec3(lengthSmallHexa / 2, lengthSmallHexa * sqrt(3) / 2, 0), colorSmallHexa),
        VertexFormat(center + glm::vec3(-lengthSmallHexa / 2, lengthSmallHexa * sqrt(3) / 2, 0), colorSmallHexa),
        VertexFormat(center + glm::vec3(-lengthSmallHexa, 0, 0), colorSmallHexa),
        VertexFormat(center + glm::vec3(-lengthSmallHexa / 2, -lengthSmallHexa * sqrt(3) / 2, 0), colorSmallHexa),
        VertexFormat(center + glm::vec3(lengthSmallHexa / 2, -lengthSmallHexa * sqrt(3) / 2, 0), colorSmallHexa)
    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices = {1, 2, 3, 1, 3, 4, 1, 4, 5, 1, 5, 6,
    0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 7};

    

    if (!fill) {
        enemy->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    enemy->InitFromData(vertices, indices);
    return enemy;
}

Mesh* object2D_t1::CreateStar(
    const std::string &name,
    glm::vec3 centerStar,
    glm::vec3 color,
    float outerRadius,
    float innerRadius,
    bool fill)
{

    glm::vec3 center = centerStar;
    
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center, color)
    };

    for(int i = 0; i < 5; i++) {
        vertices.push_back(VertexFormat(center + glm::vec3(outerRadius * cos(2 * M_PI * i / 5 + M_PI/2), outerRadius * sin(2 * M_PI * i / 5 + M_PI/2), 0), color));
    }

    for(int i = 0; i < 5; i++)
    {
        vertices.push_back(VertexFormat(center + glm::vec3(innerRadius * cos(2 * M_PI * i / 5 + M_PI / 5 + M_PI/2), innerRadius * sin(2 * M_PI * i / 5 + M_PI / 5 + M_PI/2), 0), color));
    }
    
    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices = {0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 6, 10, 1, 6, 6, 2, 7, 7, 3, 8, 8, 4, 9, 9, 5, 10};
    
    if (!fill) {
        enemy->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    enemy->InitFromData(vertices, indices);
    return enemy;
}