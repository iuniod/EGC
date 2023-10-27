#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    red = 0;
    green = 0;
    blue = 0;

    contor_mesh = 0;
    contor_color = 0;

    status = false;
    grounded = false;

    position = glm::vec3(0, 0, 0);
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    {
        changing_mesh = new Mesh("changing_mesh");
        changing_mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[changing_mesh->GetMeshID()] = changing_mesh;
    }
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(red, green, blue, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(meshes["changing_mesh"], position);
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    float speed = 3;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            position.z -= deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            position.x -= deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            position.z += deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            position.x += deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            position.y -= deltaTime * speed;
        }
        
        if (window->KeyHold(GLFW_KEY_E)) {
            position.y += deltaTime * speed;
        }
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        red = 0.5411764705882353;
        green = 0.1686274509803922;
        blue = 0.8863745098039216;
    }

    if (key == GLFW_KEY_G) {
        // TODO(student): Change the values of the color components.
        red = contor_color == 0 ? 0.5411764705882353 : 0.0f;
        green = contor_color == 0 ? 0.1686274509803922 : 0.0f;
        blue = contor_color == 0 ? 0.8863745098039216 : 0.0f;

        contor_color = (contor_color + 1) % 2;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_H) {
        changing_mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
                                changing_mesh_values[contor_mesh]);
        meshes[changing_mesh->GetMeshID()] = changing_mesh;

        contor_mesh = (contor_mesh + 1) % 3;
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_F) {
        // Restore the default color
        red = 0;
        green = 0;
        blue = 0;
    }
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
