#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

#include "components/transform.h"

using namespace std;
using namespace m1;


Tema2::Tema2() : playerTank(glm::vec3(0, 0, 0), 0, 0, 0, 0, 0, 0, 3.f, 0, glm::vec3(0, 0, -1), glm::vec3(0, 0, -1))
{
}


Tema2::~Tema2()
{
}

std::string Tema2::decodeMovementIndex(int index)
{
    switch (index)
    {
    case 0:
        return "FORWARD";
    case 1:
        return "BACKWARD";
    case 2:
        return "LEFT";
    case 3:
        return "RIGHT";
    }
}

std::string Tema2::decodeTurretMoveIndex(int index)
{
    switch (index)
    {
    case 0:
        return "ROTATE_LEFT";
    case 1:
        return "ROTATE_RIGHT";
    case 2:
        return "STAY";
    }
}

float Tema2::getRandIntNum(int min, int max)
{
    std::random_device rand;
    std::mt19937 gen(rand());

    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

float Tema2::getRandFloatNum(float min, float max)
{
    std::random_device rand;
    std::mt19937 gen(rand());

    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}


void Tema2::generateBuildings()
{
    for(int i = 0; i < numBuildings; i++)
    {
        float scaleX = getRandFloatNum(5, 15);
        float scaleY = getRandFloatNum(5, 20);
        float scaleZ = getRandFloatNum(5, 15);
        
        float x = getRandFloatNum(-70,70);
        if(x < 0)
            x -= 10;
        else
            x += 10;
        float z = getRandFloatNum(-70,70);
        if(z < 0)
            z -= 10;
        else
            z += 10;
        float y = 0.5f * scaleY;
        
        glm::vec3 position;
        position.x = x;
        position.y = y;
        position.z = z;
        
        buildings.push_back(Building(position, scaleX, scaleY, scaleZ));
    }
}

void Tema2::generateEnemyTank()
{
    float x = getRandFloatNum(-70, 70);
    float z = getRandFloatNum(-70, 70);
    float y = 0;
    float tankAngle = 180.f;
    float turretAngle = 0.f;
    std::string movementState = decodeMovementIndex(getRandIntNum(0, 3));
    std::string turretState = decodeTurretMoveIndex(getRandIntNum(0, 2));
    float targetSeconds = getRandFloatNum(1, 5);
    float targetSecondsTurret = getRandFloatNum(1, 4);
    
    enemyTanks.push_back(EnemyTank(tankAngle, 3.f, turretAngle, 0, glm::vec3(x, y, z),
        glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), movementState, turretState, targetSeconds, targetSecondsTurret));
}

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera_t2();
    camera->Set(playerTank.position + glm::vec3(0.f, 4.f, 8.f), glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("ground");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "senile.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("proiectil");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "proiectil.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cladire");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader *shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    generateBuildings();

    for(int i = 0; i < numEnemyTanks; i++)
    {
        generateEnemyTank();
    }
    
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.52f, 0.87f, 0.98f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

bool Tema2::checkCollisionProjectileTank(Projectile projectile, glm::vec3 tankPosition)
{
    float distance = glm::distance(projectile.position, tankPosition);

    if(distance < projectileRadius + tankRadius)
    {
        return true;
    }
    return false;
}

bool Tema2::checkCollisionPlayerEnemy(EnemyTank enemy_tank)
{
    float distance = glm::distance(playerTank.position, enemy_tank.position);
    if(distance < tankRadius * 2)
    {
        return true;
    }
    return false;
}

glm::vec3 Tema2::checkCollisionTankBuilding(glm::vec3& tank_position, bool isPlayer)
{
    for(const auto& building : buildings)
    {
        float minX = building.position.x - building.scaleX / 2;
        float maxX = building.position.x + building.scaleX / 2;
        float minZ = building.position.z - building.scaleZ / 2;
        float maxZ = building.position.z + building.scaleZ / 2;
        float minY = building.position.y - building.scaleY / 2;
        float maxY = building.position.y + building.scaleY / 2;

        float x = std::max(minX, std::min(tank_position.x, maxX));
        float y = std::max(minY, std::min(tank_position.y, maxY));
        float z = std::max(minZ, std::min(tank_position.z, maxZ));

        if(glm::distance(glm::vec3(x, y, z), tank_position) < tankRadius)
        {
            float P = tankRadius - glm::distance(tank_position, glm::vec3(x, y, z));
            glm::vec3 direction = glm::normalize(tank_position - glm::vec3(x, y, z));
            tank_position += direction * P * 0.5f;
            if(isPlayer)
                camera->position += direction * P * 0.5f;
            break;
        }
    }

    return tank_position;
}

bool Tema2::checkCollisionProjectileBuilding(const Projectile& projectile)
{
    for(const auto& building : buildings)
    {
        float minX = building.position.x - building.scaleX / 2;
        float maxX = building.position.x + building.scaleX / 2;
        float minZ = building.position.z - building.scaleZ / 2;
        float maxZ = building.position.z + building.scaleZ / 2;
        float minY = building.position.y - building.scaleY / 2;
        float maxY = building.position.y + building.scaleY / 2;

        float x = std::max(minX, std::min(projectile.position.x, maxX));
        float y = std::max(minY, std::min(projectile.position.y, maxY));
        float z = std::max(minZ, std::min(projectile.position.z, maxZ));

        if(glm::distance(glm::vec3(x, y, z), projectile.position) < projectileRadius)
        {
            return true;
        }
    }

    return false;
}

bool Tema2::checkTankNearby(EnemyTank enemy_tank)
{
    float distance = glm::distance(playerTank.position, enemy_tank.position);
    if(distance < tankRadius * 2 + 10)
    {
        return true;
    }
    return false;
}

void Tema2::checkCollisionEnemyEnemy(EnemyTank& enemy_tank)
{
    for(int i = 0; i < numEnemyTanks; i++)
    {

        float distance = glm::distance(enemy_tank.position, enemyTanks[i].position);
        if(distance)
        {
            if(distance < tankRadius * 2)
            {
                float P = (2 * tankRadius) - distance;
                glm::vec3 direction = glm::normalize(enemy_tank.position - enemyTanks[i].position);
                enemy_tank.position += direction * P * 0.5f;
                enemyTanks[i].position -= direction * P * 0.5f;
            }
        }
    }
}

void Tema2::RenderEnemyProjectiles(float deltaTimeSeconds)
{
    for(int enemy_index = 0; enemy_index < numEnemyTanks; enemy_index++)
    {
        for(int i = 0; i < enemyTanks[enemy_index].projectiles.size(); i++)
        {
            if(checkCollisionProjectileBuilding(enemyTanks[enemy_index].projectiles[i]))
            {
                enemyTanks[enemy_index].projectiles[i].hit = true;
            }
            
            if(enemyTanks[enemy_index].projectiles[i].time >= 5.f || enemyTanks[enemy_index].projectiles[i].hit == true)
            {
                enemyTanks[enemy_index].projectiles.erase(enemyTanks[enemy_index].projectiles.begin() + i);

            } else if(enemyTanks[enemy_index].projectiles[i].hit == false) {
                enemyTanks[enemy_index].projectiles[i].position -= deltaTimeSeconds * enemyTanks[enemy_index].projectiles[i].forward;
                enemyTanks[enemy_index].projectiles[i].time += deltaTimeSeconds;
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemyTanks[enemy_index].projectiles[i].position);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[enemy_index].projectiles[i].angle), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
                RenderMesh(meshes["proiectil"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0, 0));
            
                if(checkCollisionProjectileTank(enemyTanks[enemy_index].projectiles[i], playerTank.position))
                {
                    if(playerTank.hp)
                    {
                        playerTank.hp -= damagePerHit;
                    }
                    enemyTanks[enemy_index].projectiles[i].hit = true;
                }
            }
        }
    }
}


void Tema2::RenderEnemies(float deltaTimeSeconds)
{
    for(int i = 0; i < numEnemyTanks; i++)
    {
        enemyTanks[i].position = checkCollisionTankBuilding(enemyTanks[i].position);
        checkCollisionEnemyEnemy(enemyTanks[i]);
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["senile"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), enemyTanks[i].hp);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,  enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["corp"], shaders["MyShader"], modelMatrix, glm::vec3(0.54f, 0, 0), enemyTanks[i].hp, true);
        }
        
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,  enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].turretAngle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["turela"], shaders["MyShader"], modelMatrix, glm::vec3(0.69f, 0.13f, 0.13f), enemyTanks[i].hp, true);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,  enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].turretAngle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["tun"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), enemyTanks[i].hp, true);
        }

        if(enemyTanks[i].hp)
        {
            if(checkTankNearby(enemyTanks[i]))
            {
                enemyTanks[i].cooldown += deltaTimeSeconds;
                glm::vec3 dir = (enemyTanks[i].position - playerTank.position);
                float rotationAngle = atan2(dir.x, dir.z);
            
                enemyTanks[i].turretAngle = DEGREES(rotationAngle);
                enemyTanks[i].forwardTurret = dir;
                if(enemyTanks[i].cooldown >= 3.f)
                {
                    enemyTanks[i].projectiles.push_back(Projectile(false, enemyTanks[i].position, enemyTanks[i].forwardTurret, enemyTanks[i].turretAngle));
                    enemyTanks[i].cooldown = 0;
                }
            }
            enemyTanks[i].updateTankPosition(deltaTimeSeconds);
        
            enemyTanks[i].updateTimerMove(deltaTimeSeconds);
            if(enemyTanks[i].timerMove > enemyTanks[i].targetSecondsTank)
            {
                std::string new_state = decodeMovementIndex(getRandIntNum(0, 3));
                float new_target_seconds;
                if(new_state == "FORWARD" || new_state == "BACKWARD")
                {
                    new_target_seconds = getRandFloatNum(4.f, 6.f);
                } else
                {
                    new_target_seconds = getRandFloatNum(0.f, 1.f);
                }
                enemyTanks[i].updateMovementState(new_state, new_target_seconds);
            }
        }
    }
}

void Tema2::RenderProjectiles(float deltaTimeSeconds)
{
    for(int i = 0; i < playerProjectiles.size(); i++)
    {
        if(checkCollisionProjectileBuilding(playerProjectiles[i]))
        {
            playerProjectiles[i].hit = true;
        }
        if(playerProjectiles[i].time >= 5.f || playerProjectiles[i].hit == true)
        {
            playerProjectiles.erase(playerProjectiles.begin() + i);
        }
        
        else if(playerProjectiles[i].hit == false)
        {
            glm::vec3 direction = glm::normalize(glm::vec3(playerProjectiles[i].forward.x, 0, playerProjectiles[i].forward.z));
            playerProjectiles[i].position += deltaTimeSeconds * 15 * direction;
            playerProjectiles[i].time += deltaTimeSeconds;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, playerProjectiles[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(playerProjectiles[i].angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["proiectil"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0, 0));
            
            for(int j = 0; j < numEnemyTanks; j++)
            {
                if(checkCollisionProjectileTank(playerProjectiles[i], enemyTanks[j].position))
                {
                    if(enemyTanks[j].hp)
                    {
                        enemyTanks[j].hp -= damagePerHit;
                    }

                    playerProjectiles[i].hit = true;
                    break;
                    
                }
            }
        }
    }
}




void Tema2::Update(float deltaTimeSeconds)
{
    timer += deltaTimeSeconds;
    if(timer > 120.f)
    {
        for(int i = 0; i < numEnemyTanks; i++)
            if(enemyTanks[i].hp == 0)
                score++;
        
        std::cout << "GAME OVER (TIME IS UP)\n" << "SCORE: " << score << std::endl;
        exit(0);
    }

    if(playerTank.hp == 0)
    {
        std::cout << "GAME OVER (PLAYER TANK WAS DESTROYED)\n" ;
        exit(0);
    }

    playerTank.position = checkCollisionTankBuilding(playerTank.position, true);
    
    playerTank.cooldown += deltaTimeSeconds;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(500, 0, 500));
        RenderMesh(meshes["ground"], shaders["MyShader"], modelMatrix, glm::vec3(0.46f, 0.53f, 0.6f));
    }
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["senile"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), playerTank.hp);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["corp"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.39f, 0), playerTank.hp, true);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["turela"], shaders["MyShader"], modelMatrix, glm::vec3(0.13f, 0.54f, 0.13f), playerTank.hp, true);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["tun"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), playerTank.hp, true);
    }

    
    
    for(int i = 0; i < numBuildings; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(buildings[i].position));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(buildings[i].scaleX, buildings[i].scaleY, buildings[i].scaleZ));
        RenderMesh(meshes["cladire"], shaders["MyShader"], modelMatrix, glm::vec3(0.09f, 0.09f, 0.49f));
    }
    
    RenderProjectiles(deltaTimeSeconds);
    RenderEnemies(deltaTimeSeconds);
    RenderEnemyProjectiles(deltaTimeSeconds);
    

    for(int i = 0; i < enemyTanks.size(); i++)
    {
        if(checkCollisionPlayerEnemy(enemyTanks[i]))
        {
            float P = (2 * tankRadius) - glm::distance(playerTank.position, enemyTanks[i].position);
            glm::vec3 direction = glm::normalize(playerTank.position - enemyTanks[i].position);
            playerTank.position += direction * P * 0.5f;
            enemyTanks[i].position -= direction * P * 0.5f;
            camera->position += direction * P * 0.5f;
        }
    }
}


void Tema2::FrameEnd()
{
    
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 color, int hp, bool displace)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
    
    int location_model = glGetUniformLocation(shader->program, "Model");
    
    glUniformMatrix4fv(location_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    int location_view = glGetUniformLocation(shader->program, "View");
    
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(location_view, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    
    int location_projection = glGetUniformLocation(shader->program, "Projection");
    
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int location = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    int hp_location = glGetUniformLocation(shader->program, "hp");
    glUniform1f(hp_location, hp);

    int displace_location = glGetUniformLocation(shader->program, "displace");
    glUniform1i(displace_location, displace);
    
    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
        if(window->KeyHold(GLFW_KEY_W))
        {
            playerTank.position += 6 * deltaTime * playerTank.forwardTank;
            camera->RotateFirstPerson_OY(-camera->angle);
            camera->MoveForward(deltaTime * 6);
            camera->RotateFirstPerson_OY(camera->angle);

        }
        if(window->KeyHold(GLFW_KEY_S))
        {
            playerTank.position -= 6 * deltaTime * playerTank.forwardTank;
            camera->RotateFirstPerson_OY(-camera->angle);
            camera->MoveForward(-deltaTime * 6);
            camera->RotateFirstPerson_OY(camera->angle);
        }
        if(window->KeyHold(GLFW_KEY_A))
        {
            playerTank.angle += deltaTime * 100;
            camera->RotateThirdPerson_OY(RADIANS(deltaTime * 100));
            playerTank.forwardTank = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTank, 1)));
            playerTank.forwardTurret = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTurret, 1)));
        }
        if(window->KeyHold(GLFW_KEY_D))
        {
            playerTank.angle -= deltaTime * 100;
            camera->RotateThirdPerson_OY(RADIANS(-deltaTime * 100));
            playerTank.forwardTank = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTank, 1)));
            playerTank.forwardTurret = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-deltaTime * 100), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTurret, 1)));
        
        }
    
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        camera->angle += deltaX * 0.01f;
        camera->RotateThirdPerson_OY(deltaX * 0.01f);
 
    } else
    {
        if(deltaX > 0)
        {
            playerTank.turretAngle -= 2.f;
            playerTank.forwardTurret = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-2.f), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTurret, 1)));
        } else
        {
            playerTank.turretAngle += 2.f;
            playerTank.forwardTurret = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(2.f), glm::vec3(0, 1, 0)) * glm::vec4(playerTank.forwardTurret, 1)));
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(button == 1 && playerTank.cooldown > 1.f)
    {
        playerProjectiles.push_back(Projectile(false, playerTank.position, playerTank.forwardTurret, playerTank.turretAngle + playerTank.angle));
        playerTank.cooldown = 0;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
