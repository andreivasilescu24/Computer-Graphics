#pragma once

#include "Building.h"
#include "EnemyTank.h"
#include "Projectile.h"
#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "Tank.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        float getRandFloatNum(float min, float max);
        void generateBuildings();
        void generateEnemyTank();
        void RenderEnemies(float deltaTimeSeconds);
        float getRandIntNum(int min, int max);

     protected:
        implemented::Camera_t2 *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float fov = RADIANS(60);
        Tank playerTank;
        std::vector<Projectile> playerProjectiles;
        std::vector<Building> buildings;
        std:: vector<EnemyTank> enemyTanks;
        int numBuildings = 20;
        int numEnemyTanks = 20;
        float timer = 0;
        
    };
}   // namespace m1
