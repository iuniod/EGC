#pragma once

#include <string>
#include <unordered_map>
#include <map>

#include "components/simple_scene.h"
#include "components/transform.h"

namespace m1
{
    struct Diamond {
        glm::vec3 center;
        float radius;
        glm::mat4 modelMatrix;
    };

    struct Star {
        glm::vec3 center;
        glm::mat3 modelMatrix;
        int type;
        float translateX, translateY;
        float angle_rotate;
    };

    struct Enemy {
        glm::vec3 center;
        float radius;
        int type;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        int no_lives;
    };

    struct Rocket {
        Mesh *mesh;
        glm::vec3 center;
        float side;
        glm::mat3 modelMatrix;
        int type;
        int no_diamonds;
        bool is_on_grid = false;
        bool is_destroyed = false;
        bool has_enemy = false;
        int row;
        float stars_cooldown;
    };

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

        void DrawScene(float deltaTimeSeconds);
        bool Tema1::DecreaseLifes(Enemy &enemy, int row, int pos);
        bool RemoveRocket(Rocket &rocket);
        void ShootIfPossible(Rocket &rocket, float deltaTimeSeconds);
        bool CheckIfHit(Star &star, int row);
        bool KillEnemy(Enemy &enemy, int row, int pos);
        void RenderShaderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D *texture);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        std::unordered_map<std::string, Texture2D *> mapTextures;

        float cx, cy;
        glm::mat3 modelMatrix, heart_modelMatrix;
        float squareSide;
        glm::vec3 corner;
        glm::ivec2 resolution;
        float heart_radius, diamond_radius, star_radius;
        int lives, no_diamonds;
        float diamond_cooldown, diamond_time_elapsed;
        float enemies_general_cooldown, diamonds_general_cooldown;
        float enemies_cooldown;
        int killed_enemies = 0;

        // vector of rockets
        std::vector<Rocket> rockets;
        Rocket selected_rocket;
        bool rocket_selected;

        // vector of diamonds
        std::vector<Diamond> diamonds;

        // map of enemies on rows
        std::map<int, std::vector<Enemy>> enemies;

        // map of stars on rows
        std::map<int, std::vector<Star>> stars;

        // grid of rockets
        Rocket rockets_grid[3][3];

        // colors
        glm::vec3 light_pink = glm::vec3(1, 0.7137f, 0.756f);
        glm::vec3 hot_pink = glm::vec3(1, 0.4117f, 0.7058f);
        glm::vec3 light_blue = glm::vec3(0.6784f, 0.847f, 0.902f);
        glm::vec3 light_red = glm::vec3(0.62f, 0.0862f, 0.2431f);
        glm::vec3 light_green = glm::vec3(0.6784f, 0.902f, 0.6784f);
        glm::vec3 light_yellow = glm::vec3(1, 1, 0.4117f);
        glm::vec3 light_gray = glm::vec3(0.6588f, 0.6588f, 0.6588f);
        glm::vec3 dark_gray = glm::vec3(0.4117f, 0.4117f, 0.4117f);
        glm::vec3 light_orange = glm::vec3(1, 0.7137f, 0.2313f);
    };
}   // namespace teme
