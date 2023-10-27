#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
     public:
        Lab1();
        ~Lab1();

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

        // TODO(student): Class variables go here
        GLclampf red;
        GLclampf green;
        GLclampf blue;

        Mesh *changing_mesh;
        std::string changing_mesh_values[3] = {"box.obj", "teapot.obj", "sphere.obj"};
        int contor_mesh = 0;

        glm::vec3 position;

        bool status;
        int initial_pos;
        bool grounded;

        int contor_color;
    };
}   // namespace m1
