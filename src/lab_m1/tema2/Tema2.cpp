#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "components/transform.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2() : playerTank(0, 0, 0, 0, 0, false)
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera_t2();
    camera->Set(glm::vec3(playerTank.x, playerTank.y + 3, playerTank.z + 5), glm::vec3(playerTank.x, playerTank.y, playerTank.z), glm::vec3(0, 1, 0));

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

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters

    {
        Shader *shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
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


void Tema2::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 0, 100));
        RenderMesh(meshes["ground"], shaders["MyShader"], modelMatrix, glm::vec3(0.19f, 0.8f, 0.19f));
    }
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerTank.x, playerTank.y, playerTank.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["senile"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerTank.x, playerTank.y, playerTank.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["corp"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.39f, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerTank.x, playerTank.y, playerTank.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["turela"], shaders["MyShader"], modelMatrix, glm::vec3(0.13f, 0.54f, 0.13f));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerTank.x, playerTank.y, playerTank.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerTank.turretAngle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderMesh(meshes["tun"], shaders["MyShader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    // if (renderCameraTarget)
    // {
    //     glm::mat4 modelMatrix = glm::mat4(1);
    //     modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    //     modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    //     RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
    // }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 color)
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
            playerTank.x -= deltaTime * 2 * sin(RADIANS(playerTank.angle));
            playerTank.z -= deltaTime * 2 * cos(RADIANS(playerTank.angle));
            camera->MoveForward(deltaTime * 2);
        }
        if(window->KeyHold(GLFW_KEY_S))
        {
            playerTank.x += deltaTime * 2 * sin(RADIANS(playerTank.angle));
            playerTank.z += deltaTime * 2 * cos(RADIANS(playerTank.angle));
            camera->MoveForward(-deltaTime * 2);
        }
        if(window->KeyHold(GLFW_KEY_A))
        {
            playerTank.angle += deltaTime * 100;
            // camera->TranslateForward(-5);
            camera->RotateThirdPerson_OY(RADIANS(deltaTime * 100));
            // camera->TranslateForward(5);
        }
        if(window->KeyHold(GLFW_KEY_D))
        {
            playerTank.angle -= deltaTime * 100;
            // camera->TranslateForward(-5);
            camera->RotateThirdPerson_OY(RADIANS(-deltaTime * 100));
            // camera->TranslateForward(5);
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
    std::cout << mouseX << " " << mouseY << " " << deltaX << " " << deltaY << "\n";
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
            playerTank.turretAngle -= 2 * 0.8f;
        } else
        {
            playerTank.turretAngle += 2 * 0.8f;
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
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
