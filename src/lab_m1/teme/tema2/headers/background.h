#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <string>
#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/teme/tema2/headers/object3D.h"
#include "lab_m1/teme/tema2/headers/lab_camera.h"
#include "lab_m1/teme/tema2/headers/transform3D.h"
#include "lab_m1/teme/tema2/headers/tank.h"
#include "lab_m1/teme/tema2/headers/building.h"

namespace m1 {
    class Background : public gfxc::SimpleScene
    {
     public:
        Background();
        ~Background();

        void Init() override;
		void Update(float deltaTimeSeconds) override;
        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Tank *tank);

     private:
        void DrawHUD();
        void FrameEnd() override;
        void FrameStart() override;
        void RenderShaderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture);
        void RenderShaderMesh2(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix);

    protected:
        std::unordered_map<std::string, Texture2D *> mapTextures;
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point end;
        std::chrono::duration<double> elapsed_seconds;

        gfxc::TextRenderer *smallText, *bigText;

        std::vector<Building*> buildings;
        Tank* tank;
        std::vector<Tank*> enemies;

        int score = 50, highest_score = 50;
        bool gameOver = false;
        const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
        glm::vec4 groundCoordinates = glm::vec4(-25, -25, 25, 25);
        
        camera::Camera *camera;
        float cameraAngleOX = 0, cameraAngleOY = 0;
        glm::mat4 cameraMatrix;
        bool thirdPersonCamera = false;
        GLenum cullFace;
         GLenum polygonMode;
    };
}   // namespace m1
