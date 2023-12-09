#pragma once

#include <vector>
#include <unordered_map>
#include "lab_m1/teme/tema2/headers/background.h"


namespace m1 {
    class Tema2 : public Background
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void Update(float deltaTimeSeconds) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

    protected:
        glm::vec3 cameraOffset = glm::vec3(0, 3, -0.5f);
        glm::vec3 up = glm::vec3(0, 4, 0);
        int speed;
        float cameraAngle = 0.95f;
    };
}   // namespace m1
