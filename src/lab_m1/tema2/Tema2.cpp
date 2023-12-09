#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

#include "components/transform.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


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

// x 5 15
// y 10 25
// z 5 15

void Tema2::generateBuildings()
{
    for(int i = 0; i < numBuildings; i++)
    {
        float scaleX = getRandFloatNum(5, 15);
        float scaleY = getRandFloatNum(5, 20);
        float scaleZ = getRandFloatNum(5, 15);
        
        float x = getRandFloatNum(-70, 70);
        float z = getRandFloatNum(-70, 70);
        float y = 0.5f * scaleY;
        
        buildings.push_back(Building(x, y, z, scaleX, scaleY, scaleZ));
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
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), movementState, turretState, targetSeconds, targetSecondsTurret));
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

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters

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

bool Tema2::checkCollisionProjectileTank(Projectile projectile, EnemyTank enemy_tank)
{
    float distance = glm::distance(projectile.position, enemy_tank.position);

    if(distance < projectileRadius + tankRadius)
    {
        return true;
    }
    return false;
}

bool Tema2::checkCollisionPlayerEnemy(EnemyTank enemy_tank)
{
    float distance = glm::distance(playerTank.position, enemy_tank.position);
    if(distance < tankRadius + tankRadius)
    {
        std::cout << "COLLISION TANK TANK" << std::endl;
        return true;
    }
    return false;
}

void Tema2::RenderEnemies(float deltaTimeSeconds)
{
    for(int i = 0; i < numEnemyTanks; i++)
    {
        enemyTanks[i].updateTankPosition(deltaTimeSeconds);
        enemyTanks[i].updateTurretPosition();
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
            RenderMesh(meshes["corp"], shaders["MyShader"], modelMatrix, glm::vec3(0.54f, 0, 0), enemyTanks[i].hp);
        }

        if(enemyTanks[i].checkTankNearby(playerTank, tankRadius))
        {
            std::cout << "TANK NEARBY" << std::endl;
        }
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,  enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].turretAngle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["turela"], shaders["MyShader"], modelMatrix, glm::vec3(0.69f, 0.13f, 0.13f), enemyTanks[i].hp);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,  enemyTanks[i].position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].turretAngle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            RenderMesh(meshes["tun"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), enemyTanks[i].hp);
        }

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

        enemyTanks[i].updateTimerTurretMove(deltaTimeSeconds);
        if(enemyTanks[i].timerTurretMove > enemyTanks[i].targetSecondsTurret)
        {
            enemyTanks[i].turretState = decodeTurretMoveIndex(getRandIntNum(0, 2));
            if(enemyTanks[i].turretState == "ROTATE_LEFT" || enemyTanks[i].turretState == "ROTATE_RIGHT")
            {
                enemyTanks[i].targetSecondsTurret = getRandFloatNum(0.5f, 2.f);
            } else
            {
                enemyTanks[i].targetSecondsTurret = getRandFloatNum(2.f, 4.f);
            }
            enemyTanks[i].timerTurretMove = 0;
        }
    }
}

void Tema2::RenderProjectiles(float deltaTimeSeconds)
{
    for(int i = 0; i < playerProjectiles.size(); i++)
    {
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
                if(checkCollisionProjectileTank(playerProjectiles[i], enemyTanks[j]))
                {
                    enemyTanks[j].hp -= damagePerHit;
                    playerProjectiles[i].hit = true;
                    // if(enemyTanks[j].hp == 0)
                    // {
                    //     enemyTanks.erase(enemyTanks.begin() + j);
                    //     numEnemyTanks--;
                    // }
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
        exit(0);
    }
    
    playerTank.cooldown += deltaTimeSeconds;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(200, 0, 200));
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
        RenderMesh(meshes["corp"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.39f, 0), playerTank.hp);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["turela"], shaders["MyShader"], modelMatrix, glm::vec3(0.13f, 0.54f, 0.13f), playerTank.hp);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, playerTank.position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["tun"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), playerTank.hp);
    }

    
    
    for(int i = 0; i < numBuildings; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(buildings[i].x, buildings[i].y, buildings[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(buildings[i].scaleX, buildings[i].scaleY, buildings[i].scaleZ));
        RenderMesh(meshes["cladire"], shaders["MyShader"], modelMatrix, glm::vec3(0.09f, 0.09f, 0.49f));
    }
    
    RenderProjectiles(deltaTimeSeconds);
    RenderEnemies(deltaTimeSeconds);

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
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 color, int hp)
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
    
    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_O)) {
            projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 200.0f);
        }

        if(window->KeyHold(GLFW_KEY_P)) {
            projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        }

        if(window->KeyHold(GLFW_KEY_N))
        {
            fov -= deltaTime * 0.5;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
        }

        if(window->KeyHold(GLFW_KEY_M))
        {
            fov += deltaTime * 0.5;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
        }

        if(window->KeyHold(GLFW_KEY_J))
        {
            
        }

        if(window->KeyHold(GLFW_KEY_K))
        {
            
        }
    } else
    {
        if(window->KeyHold(GLFW_KEY_W))
        {
            playerTank.position += 4 * deltaTime * playerTank.forwardTank;
            camera->MoveForward(deltaTime * 4);
        }
        if(window->KeyHold(GLFW_KEY_S))
        {
            playerTank.position -= 4 * deltaTime * playerTank.forwardTank;
            camera->MoveForward(-deltaTime * 4);
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

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.
    

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
        }
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
