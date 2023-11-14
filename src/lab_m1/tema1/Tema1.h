#pragma once

#include "Enemy.h"
#include "Gun.h"
#include "MatrixSquare.h"
#include "GunSquare.h"
#include "Life.h"
#include "ShootingStar.h"
#include "Star.h"
#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        float getRandTimeInterval(float min, float max);
        int getRandIndex(int min, int max);
        void spawnEnemy(float timeSpawnEnemy);
        std::string decode_color(int enemyColor);
        void spawnCollectableStars(float spawnIndicator);
        void triggerGunsByColor(std::string color, int lineIndex, float enemyX);
        void detectCollisionStarEnemy(ShootingStar shooting_star, int index);
        void disableGunsByColor(std::string color, int lineIndex);
        void detectCollisionEnemyGun(Enemy enemy);
        int costByGunColor(std::string color);




    protected:
        glm::mat3 modelMatrix;
        float angularStep;
        
        float squareSide = 100;
        float gunWidth = 80;
        float starRadix = 30, hexagonRadix = 30;
        
        float spawnEnemyX = 1350, spawnEnemyY = 409.5f;
        float diffPixelsEnemyY = 162;
        int enemyLine;
        int enemyColor;
        float timeSpawnEnemy;

        float gunSquareDownY = 154, gunSquareUpY = 36, leftXgunSquares = 39;
        float gunSquareLength = 118, distSquares = 56;
        
        float startingXMatrixSquares = 90, startingYMatrixSquares = 248;
        float matrixSquareLength = 125, distMatrixSquares = 37;

        float startingXLives = 825, distLives = 50, lifeSquareLength = 75;

        bool drag = false;
        std::string currDraggedGunColor = "";
        float mouseMoveX, mouseMoveY;

        int lives = 3;
        int numCollectedStars = 5;
        int starsPerLine = 14;
        int remainingCollectedStars;
        int collectedStarsLines;
        int differenceBetweenStars = 60;
        float timeSpawnCollectableStars;

        float startingXCollectedStars = 2080, startingYCollectedStars = 1400;
        
        std::vector<GunSquare> gunSquares;
        std::vector<std::vector<MatrixSquare>> matrixSquares;
        std::vector<Enemy> enemies;
        std::vector<Gun> gunsPlaced;
        std::vector<Life> lifeSquares;
        std::vector<Star> starsCollectable;
        std::vector<ShootingStar> shootingStars;
    };
}   // namespace m1
