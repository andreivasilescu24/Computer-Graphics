#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <random>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D_t1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    
    timeSpawnEnemy = 0;
    timeSpawnCollectableStars = 0;

    for(int i = 0; i < 4; i++)
    {
        gunSquares.push_back(GunSquare(i, leftXgunSquares + i * (distSquares + gunSquareLength), leftXgunSquares + i * (distSquares + gunSquareLength) + gunSquareLength, gunSquareUpY, gunSquareDownY));
    }
    
    for(int i = 0; i < 3; i++) {
        std::vector<MatrixSquare> matrixRow;
        for(int j = 0; j < 3; j++) {
            float leftX = startingXMatrixSquares + j * (distMatrixSquares + matrixSquareLength);
            float rightX = startingXMatrixSquares + j * (distMatrixSquares + matrixSquareLength) + matrixSquareLength;
            float upY = startingYMatrixSquares + i * (distMatrixSquares + matrixSquareLength);
            float downY = startingYMatrixSquares + i * (distMatrixSquares + matrixSquareLength) + matrixSquareLength;
    
            matrixRow.push_back(MatrixSquare(false, leftX, rightX, upY, downY));
        }
        matrixSquares.push_back(matrixRow);
    }

    for(int i = 0; i < 3; i++)
    {
        lifeSquares.push_back(Life(startingXLives + i * (distLives + lifeSquareLength), 585));
    }
    
    // GUNS SQUARES
    Mesh* squareGunOrange = object2D_t1::CreateSquare("squareGunOrange", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(squareGunOrange);
    Mesh* squareGunBlue = object2D_t1::CreateSquare("squareGunBlue", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(squareGunBlue);
    Mesh* squareGunYellow = object2D_t1::CreateSquare("squareGunYellow", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(squareGunYellow);
    Mesh* squareGunPurple = object2D_t1::CreateSquare("squareGunPurple", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(squareGunPurple);

    // GUNS
    Mesh* gunOrange = object2D_t1::CreateGun("gunOrange", glm::vec3(0, 0, 1),115, 80, glm::vec3(1, 0.36f, 0.01f), true);
    AddMeshToList(gunOrange);
    Mesh* gunBlue = object2D_t1::CreateGun("gunBlue", glm::vec3(0, 0, 1),115, 80, glm::vec3(0, 0, 1), true);
    AddMeshToList(gunBlue);
    Mesh* gunYellow = object2D_t1::CreateGun("gunYellow", glm::vec3(0,  0, 1), 115, 80, glm::vec3(1, 0.76f, 0.04f), true);
    AddMeshToList(gunYellow);
    Mesh* gunPurple = object2D_t1::CreateGun("gunPurple", glm::vec3(0, 0, 1), 115, 80, glm::vec3(0.63f, 0.09f, 1), true);
    AddMeshToList(gunPurple);

    // LIVES
    Mesh* lifeSquare = object2D_t1::CreateSquare("lifeSquare", corner, 75, glm::vec3(1, 0, 0), true);
    AddMeshToList(lifeSquare);

    // PLAYABLE AREA
    Mesh* borderRectangle = object2D_t1::CreateSquare("borderRectangle", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(borderRectangle);

    // MATRIX SQUARE
    Mesh* matrixSquare = object2D_t1::CreateSquare("matrixSquare", corner, 125, glm::vec3(0.596f, 1, 0.596f), true);
    AddMeshToList(matrixSquare);

    // ENEMY
    Mesh* enemyOrange = object2D_t1::CreateEnemy("enemyOrange", glm::vec3(0, 0, 3), 30, 20, glm::vec3(1, 0.36f, 0.01f), glm::vec3(0, 1, 1), true);
    AddMeshToList(enemyOrange);
    Mesh* enemyBlue = object2D_t1::CreateEnemy("enemyBlue", glm::vec3(0, 0, 3), 30, 20, glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), true);
    AddMeshToList(enemyBlue);
    Mesh* enemyYellow = object2D_t1::CreateEnemy("enemyYellow", glm::vec3(0, 0, 3), 30, 20, glm::vec3(1, 0.76f, 0.04f), glm::vec3(0, 1, 1), true);
    AddMeshToList(enemyYellow);
    Mesh* enemyPurple = object2D_t1::CreateEnemy("enemyPurple", glm::vec3(0, 0, 3), 30, 20, glm::vec3(0.63f, 0.09f, 1), glm::vec3(0, 1, 1), true);
    AddMeshToList(enemyPurple);
    

    // STAR
    Mesh* starCollectable = object2D_t1::CreateStar("starCollectable", glm::vec3(0, 0, 5), glm::vec3(0.98f, 0.17f, 0.81f), 30, 10, true);
    AddMeshToList(starCollectable);
    Mesh* starOrange = object2D_t1::CreateStar("starOrange", glm::vec3(0, 0, 4), glm::vec3(1, 0.36f, 0.01f), 30, 10, true);
    AddMeshToList(starOrange);
    Mesh* starBlue= object2D_t1::CreateStar("starBlue", glm::vec3(0, 0, 4), glm::vec3(0, 0, 1), 30, 10, true);
    AddMeshToList(starBlue);
    Mesh* starYellow = object2D_t1::CreateStar("starYellow", glm::vec3(0, 0, 4), glm::vec3(1, 0.76f, 0.04f), 30, 10, true);
    AddMeshToList(starYellow);
    Mesh* starPurple = object2D_t1::CreateStar("starPurple", glm::vec3(0, 0, 4), glm::vec3(0.63f, 0.09f, 1), 30, 10, true);
    AddMeshToList(starPurple);
    Mesh* starGrey = object2D_t1::CreateStar("starGrey", glm::vec3(0, 0, 4), glm::vec3(0.66f, 0.66f, 0.66f), 30, 10, true);
    AddMeshToList(starGrey);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

float Tema1::getRandTimeInterval(float min, float max)
{
    std::random_device rand;
    std::mt19937 gen(rand());

    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int Tema1::getRandIndex(int min, int max)
{
    std::random_device rand;
    std::mt19937 gen(rand());

    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

std::string Tema1::decode_color(int enemyColor)
{
    switch (enemyColor)
    {
        case 0:
            return "Orange";
        case 1:
            return "Blue";
        case 2:
            return "Yellow";
        case 3:
            return "Purple";
    }
}

void Tema1::spawnEnemy(float spawnIndicator) 
{
    if(spawnIndicator > getRandTimeInterval(5, 10))
    {
        timeSpawnEnemy = 0;
        enemyLine = getRandIndex(0, 2);
        enemyColor = getRandIndex(0, 3);
        string color = decode_color(enemyColor);
        enemies.push_back(Enemy(color, spawnEnemyX, spawnEnemyY - diffPixelsEnemyY * enemyLine, enemyLine));
    }
}

void Tema1::spawnCollectableStars(float spawnIndicator)
{
    if(spawnIndicator > getRandTimeInterval(5, 15))
    {
        timeSpawnCollectableStars = 0;
        int numStars = getRandIndex(2, 4);
        for(int i = 0; i < numStars; i++)
        {
            int X = getRandIndex(50, 1210);
            int Y = getRandIndex(50, 650);
            starsCollectable.push_back(Star(X, Y));
        }
    }
}

void Tema1::triggerGunsByColor(std::string color, int lineIndex, float enemyX)
{
    for(int i = 0; i < gunsPlaced.size(); i++)
    {
        if(gunsPlaced[i].shoot == false && gunsPlaced[i].color == color && gunsPlaced[i].matrixSquareI == lineIndex && gunsPlaced[i].X < enemyX)
        {
            gunsPlaced[i].shoot = true;
        }
    }
}

void Tema1::disableGunsByColor(std::string color, int lineIndex)
{
    for(int i = 0; i < gunsPlaced.size(); i++)
    {
        if(gunsPlaced[i].shoot == true && gunsPlaced[i].color == color && gunsPlaced[i].matrixSquareI == lineIndex)
        {
            gunsPlaced[i].shoot = false;
        }
    }
}

void Tema1::detectCollisionStarEnemy(ShootingStar shooting_star, int index)
{
    for(int i = 0; i < enemies.size(); i++)
    {
        if(shooting_star.color == enemies[i].color && shooting_star.matrixSquareI == enemies[i].lineIndex)
        {
            if(glm::distance(glm::vec2(shooting_star.X, shooting_star.Y), glm::vec2(enemies[i].X, enemies[i].Y)) < hexagonRadix + starRadix && enemies[i].hits < 3)
            {
                enemies[i].hits++;
                shootingStars.erase(shootingStars.begin() + index);
            }
        }
    }
}

void Tema1::detectCollisionEnemyGun(Enemy enemy)
{
    for(int i = 0; i < gunsPlaced.size(); i++)
    {
        if(enemy.lineIndex == gunsPlaced[i].matrixSquareI)
        {
            if(glm::distance(glm::vec2(enemy.X, enemy.Y), glm::vec2(gunsPlaced[i].X, 720 - gunsPlaced[i].Y)) < gunWidth / 3 + hexagonRadix)
            {
                gunsPlaced[i].isHit = true;
            }
        }
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    if(lives)
    {
        // BORDER
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(0.5f, 4.5f);
        modelMatrix *= transform2D::Translate(22.5f, 5);
        RenderMesh2D(meshes["borderRectangle"], shaders["VertexColor"], modelMatrix);
    
        // MATRIX SQUARES
        float translateMatrixX = 90;
        float translateMatrixY = 23;
    
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(translateMatrixX, translateMatrixY);
                RenderMesh2D(meshes["matrixSquare"], shaders["VertexColor"], modelMatrix);
    
                translateMatrixX += 162;
            }
            translateMatrixX = 90;
            translateMatrixY += 162;
        }
        
        // GUNS
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(95, 621);
        RenderMesh2D(meshes["gunOrange"], shaders["VertexColor"], modelMatrix);
    
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(270, 621);
        RenderMesh2D(meshes["gunBlue"], shaders["VertexColor"], modelMatrix);
    
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(445, 621);
        RenderMesh2D(meshes["gunYellow"], shaders["VertexColor"], modelMatrix);
    
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(620, 621);
        RenderMesh2D(meshes["gunPurple"], shaders["VertexColor"], modelMatrix);

        // GUNS SQUARES
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        modelMatrix *= transform2D::Translate(30, 447.5f);
        RenderMesh2D(meshes["squareGunOrange"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        modelMatrix *= transform2D::Translate(170, 447.5f);
        RenderMesh2D(meshes["squareGunBlue"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        modelMatrix *= transform2D::Translate(310, 447.5f);
        RenderMesh2D(meshes["squareGunYellow"], shaders["VertexColor"], modelMatrix);
    
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        modelMatrix *= transform2D::Translate(450, 447.5f);
        RenderMesh2D(meshes["squareGunPurple"], shaders["VertexColor"], modelMatrix);
        
        // LIVES
        for(int i = 0; i < lives; i++)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(lifeSquares[i].x, lifeSquares[i].y);
            RenderMesh2D(meshes["lifeSquare"], shaders["VertexColor"], modelMatrix);
        }

        // ENEMIES
        timeSpawnEnemy += deltaTimeSeconds;
        spawnEnemy(timeSpawnEnemy);

        for(int i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].X < -100) {
                enemies.erase(enemies.begin() + i);
            }
            else
            {
                modelMatrix = glm::mat3(1);
                if(enemies[i].hits == 3)
                {
                    enemies[i].scaleX -= deltaTimeSeconds * 3;
                    enemies[i].scaleY -= deltaTimeSeconds * 3;
                    
                    if( enemies[i].scaleX <= 0)
                    {
                        disableGunsByColor(enemies[i].color, enemies[i].lineIndex);
                        enemies.erase(enemies.begin() + i);
                    }
                } else
                {
                    enemies[i].X -= deltaTimeSeconds * 60;
                }
                modelMatrix *= transform2D::Translate(enemies[i].X, enemies[i].Y);
                modelMatrix *= transform2D::Scale(enemies[i].scaleX, enemies[i].scaleY);
                RenderMesh2D(meshes["enemy" + enemies[i].color], shaders["VertexColor"], modelMatrix);

                if(enemies[i].X <= 1280)
                {
                    triggerGunsByColor(enemies[i].color, enemies[i].lineIndex, enemies[i].X);
                    if(enemies[i].X <= 540)
                        detectCollisionEnemyGun(enemies[i]);
                }

                if(enemies[i].X <= 54 && enemies[i].isInRedZone == false)
                {
                    enemies[i].isInRedZone = true;
                    lives--;
                }
            }
        }
        
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(0.4, 0.4);
        modelMatrix *= transform2D::Translate(115, 1350);
        RenderMesh2D(meshes["starGrey"], shaders["VertexColor"], modelMatrix);
        
        int translateXBlue = 550;
        int translateXYellow = 990;
        int translateXPurple = 1430;
    
        for(int i = 0; i < 2; i++)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Scale(0.4, 0.4);
            modelMatrix *= transform2D::Translate(translateXBlue, 1350);
            RenderMesh2D(meshes["starGrey"], shaders["VertexColor"], modelMatrix);
            translateXBlue += differenceBetweenStars;

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Scale(0.4, 0.4);
            modelMatrix *= transform2D::Translate(translateXYellow, 1350);
            RenderMesh2D(meshes["starGrey"], shaders["VertexColor"], modelMatrix);
            translateXYellow += differenceBetweenStars;
        }

        for(int i = 0; i < 3; i++)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Scale(0.4, 0.4);
            modelMatrix *= transform2D::Translate(translateXPurple, 1350);
            RenderMesh2D(meshes["starGrey"], shaders["VertexColor"], modelMatrix);
            translateXPurple += differenceBetweenStars;
        }

        if(drag == true)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(mouseMoveX, 720 - mouseMoveY);
            RenderMesh2D(meshes["gun" + currDraggedGunColor], shaders["VertexColor"], modelMatrix);
        }

        // GUNS PLACED
        for(int i = 0; i < gunsPlaced.size(); i++)
        {
            modelMatrix = glm::mat3(1);
            if(matrixSquares[gunsPlaced[i].matrixSquareI][gunsPlaced[i].matrixSquareJ].emptySquare || gunsPlaced[i].isHit)
            {
                gunsPlaced[i].scaleX -= deltaTimeSeconds * 3;
                gunsPlaced[i].scaleY -= deltaTimeSeconds * 3;
            }
            modelMatrix *= transform2D::Translate(gunsPlaced[i].X, 720 - gunsPlaced[i].Y);
            modelMatrix *= transform2D::Scale(gunsPlaced[i].scaleX, gunsPlaced[i].scaleY);
            RenderMesh2D(meshes[gunsPlaced[i].name], shaders["VertexColor"], modelMatrix);
            
            // ADD SHOOTING STAR
            if(gunsPlaced[i].shoot && (gunsPlaced[i].timeBetweenStars >= 3.5f || gunsPlaced[i].timeBetweenStars == 0))
            {
                shootingStars.push_back(ShootingStar(gunsPlaced[i].X + gunWidth / 3, 720 - gunsPlaced[i].Y, 0,
                    gunsPlaced[i].color, gunsPlaced[i].matrixSquareI, gunsPlaced[i].matrixSquareJ));
                gunsPlaced[i].timeBetweenStars = 0;
            }
            gunsPlaced[i].timeBetweenStars += deltaTimeSeconds;

            if(gunsPlaced[i].scaleX <= 0)
            {
                matrixSquares[gunsPlaced[i].matrixSquareI][gunsPlaced[i].matrixSquareJ].emptySquare = false;
                matrixSquares[gunsPlaced[i].matrixSquareI][gunsPlaced[i].matrixSquareJ].isFull = false;
                for(int stars_index = 0; stars_index < shootingStars.size(); stars_index++)
                {
                    if(shootingStars[stars_index].color == gunsPlaced[i].color &&
                            shootingStars[stars_index].matrixSquareI == gunsPlaced[i].matrixSquareI &&
                            shootingStars[stars_index].matrixSquareJ == gunsPlaced[i].matrixSquareJ &&
                            shootingStars[stars_index].X >= 1300)
                        shootingStars.erase(shootingStars.begin() + stars_index);
                }
                gunsPlaced.erase(gunsPlaced.begin() + i);
            }
        }

        // STARS COLLECTED
        timeSpawnCollectableStars += deltaTimeSeconds;
        spawnCollectableStars(timeSpawnCollectableStars);
        
        for(int i = 0; i < starsCollectable.size(); i++)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(starsCollectable[i].X, starsCollectable[i].Y);
            RenderMesh2D(meshes["starCollectable"], shaders["VertexColor"], modelMatrix);
        }
        
        if(numCollectedStars % starsPerLine)
            collectedStarsLines = numCollectedStars / starsPerLine + 1;
        else 
            collectedStarsLines = numCollectedStars / starsPerLine;

        int auxCollectedStars = numCollectedStars;
        for(int i = 0; i < collectedStarsLines; i++)
        {
            if(auxCollectedStars > starsPerLine)
                remainingCollectedStars = starsPerLine;
            else remainingCollectedStars = auxCollectedStars;
            
            for(int j = 0; j < remainingCollectedStars; j++)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Scale(0.4, 0.4);
                modelMatrix *= transform2D::Translate(startingXCollectedStars + j * differenceBetweenStars, startingYCollectedStars - i * differenceBetweenStars);
                RenderMesh2D(meshes["starGrey"], shaders["VertexColor"], modelMatrix);
            }
            auxCollectedStars -= remainingCollectedStars;
        }

        for(int i = 0; i < shootingStars.size(); i++)
        {
            modelMatrix = glm::mat3(1);
            shootingStars[i].X += deltaTimeSeconds * 250;
            shootingStars[i].angularStep += deltaTimeSeconds * 3.5f;
            modelMatrix *= transform2D::Translate(shootingStars[i].X, shootingStars[i].Y);
            modelMatrix *= transform2D::Rotate(-shootingStars[i].angularStep);
            RenderMesh2D(meshes["star" + shootingStars[i].color], shaders["VertexColor"], modelMatrix);
            detectCollisionStarEnemy(shootingStars[i], i);
        }
        
    } else
    {
        exit(0);
    }
}


void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema1::OnKeyPress(int key, int mods)
{
        
}


void Tema1::OnKeyRelease(int key, int mods)
{
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseMoveX = mouseX;
    mouseMoveY = mouseY;
}

int Tema1::costByGunColor(std::string color)
{
    if (color == "Orange") {
        return 1;
    } else if (color == "Yellow" || color == "Blue") {
        return 2;
    } else if (color == "Purple") {
        return 3;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    bool collectStar = false;
    if(button == 1)
    {
        for(int i = 0; i < starsCollectable.size(); i++)
        {
            if(mouseX <= starsCollectable[i].X + 30 && mouseX >= starsCollectable[i].X - 30)
            {
                if(720 - mouseY <= starsCollectable[i].Y + 30 && 720 - mouseY >= starsCollectable[i].Y - 30)
                {
                    collectStar = true;
                    numCollectedStars++;
                    starsCollectable.erase(starsCollectable.begin() + i);
                    break;
                }
            }
        }

        
        if(collectStar == false)
        {
            for(int i = 0; i < gunSquares.size(); i++)
            {
                float currentMinX = gunSquares[i].leftX;
                float currentMaxX = gunSquares[i].rightX;

                if(mouseX >= currentMinX && mouseX <= currentMaxX && mouseY <= gunSquareDownY && mouseY >= gunSquareUpY && costByGunColor(decode_color(gunSquares[i].index)) <= numCollectedStars)
                {
                    currDraggedGunColor = decode_color(gunSquares[i].index);
                    drag = true;
                } 
            }
        }
    } else if(button == 2)
    {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                float leftX = matrixSquares[i][j].leftX;
                float rightX = matrixSquares[i][j].rightX;
                float upY = matrixSquares[i][j].upY;
                float downY = matrixSquares[i][j].downY;

                if(mouseX >= leftX && mouseX <= rightX && mouseY <= downY && mouseY >= upY && matrixSquares[i][j].isFull)
                {
                    matrixSquares[i][j].emptySquare = true;
                    break;
                }
            }
        }
    
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if(button == 1)
    {
        bool exitIteration = false;
        if(drag)
        {
            drag = false;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    float leftX = matrixSquares[i][j].leftX;
                    float rightX = matrixSquares[i][j].rightX;
                    float upY = matrixSquares[i][j].upY;
                    float downY = matrixSquares[i][j].downY;

                    if(mouseX >= leftX && mouseX <= rightX && mouseY <= downY && mouseY >= upY && !matrixSquares[i][j].isFull)
                    {
                        exitIteration = true;
                        int gunCost = costByGunColor(currDraggedGunColor);
                        gunsPlaced.push_back(Gun(currDraggedGunColor, "gun" + currDraggedGunColor,(leftX + rightX) / 2.04f, (downY + upY) / 2, 1, 1, i, j, gunCost));
                        matrixSquares[i][j].isFull = true;
                        numCollectedStars -= gunCost;
                        break;
                    }
                }
                if(exitIteration)
                    break;
            }
            currDraggedGunColor = "";
        }
    }

}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
