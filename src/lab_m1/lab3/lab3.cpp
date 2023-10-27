#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    corner = glm::vec3(0, 0, 0);
    squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* square41 = object2D::CreateSquare("square41", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(square41);
    Mesh* square42 = object2D::CreateSquare("square42", corner, squareSide, glm::vec3(0, 1, 1), true);
    AddMeshToList(square42);
    Mesh* square43 = object2D::CreateSquare("square43", corner, squareSide, glm::vec3(1, 0, 1), true);
    AddMeshToList(square43);
    Mesh* square44 = object2D::CreateSquare("square44", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(square44);
    Mesh* square45 = object2D::CreateSquare("square45", corner, squareSide, glm::vec3(0, 1, 1), true);
    AddMeshToList(square45);
    Mesh* square46 = object2D::CreateSquare("square46", corner, squareSide, glm::vec3(1, 0, 1), true);
    AddMeshToList(square46);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!
    translateX += deltaTimeSeconds * 50;
    translateY += deltaTimeSeconds * 50;

    if (translateX > 500 || translateY > 500) {
        translateX = 0;
        translateY = 0;

        modelMatrix *= transform2D::Translate(150, 150);
        modelMatrix *= transform2D::Translate(-cx, -cy);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 150);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    scaleX += deltaTimeSeconds * 0.5;
    scaleY += deltaTimeSeconds * 0.5;

    if (scaleX > 5 || scaleY > 5) {
        scaleX = 1;
        scaleY = 1;
    }
    modelMatrix = glm::mat3(1);
    
    modelMatrix *= transform2D::Translate(300, 300);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    angularStep += deltaTimeSeconds * 5;
    modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(450, 450);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    // BONUS!
    // Render 6 squares to create a hexagon
    // Use the same transformations as above, but with different parameters
    float rotateAngle = 5 * 3.14 / 6;

    // Up
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 200);
    modelMatrix *= transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["square41"], shaders["VertexColor"], modelMatrix);

    // Up-Right
    modelMatrix3 = modelMatrix;
    modelMatrix3 *= transform2D::Rotate(rotateAngle);
    RenderMesh2D(meshes["square43"], shaders["VertexColor"], modelMatrix3);

    // Down-Right
    modelMatrix2 = modelMatrix;
    modelMatrix2 *= transform2D::Translate(- (squareSide / 2), -(1.73 * squareSide / 2));
    modelMatrix2 *= transform2D::Rotate(-rotateAngle);
    RenderMesh2D(meshes["square42"], shaders["VertexColor"], modelMatrix2);

    // Down
    modelMatrix4 = modelMatrix;
    modelMatrix4 *= transform2D::Translate(0, - (2.73 * squareSide));
    RenderMesh2D(meshes["square44"], shaders["VertexColor"], modelMatrix4);

    // Down-Left
    modelMatrix5 = modelMatrix;
    modelMatrix5 *= transform2D::Translate(4.73 * squareSide / 2, -(2.73 * squareSide / 2));
    modelMatrix5 *= transform2D::Rotate(rotateAngle);
    RenderMesh2D(meshes["square45"], shaders["VertexColor"], modelMatrix5);

    // Up-Left
    modelMatrix6 = modelMatrix;
    modelMatrix6 *= transform2D::Translate((3.73 * squareSide / 2), (squareSide / 2));
    modelMatrix6 *= transform2D::Rotate(-rotateAngle);
    RenderMesh2D(meshes["square46"], shaders["VertexColor"], modelMatrix6);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
