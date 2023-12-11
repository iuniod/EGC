#include "lab_m1/teme/tema2/headers/tema2.h"

using namespace std;
using namespace m1;
using namespace glm;

// Internal functions used
float dot(vec2 a, vec2 b);
bool pointInRectangle(vec2 point, vector<vec2> rectangleVertices);

Tema2::Tema2() {
}

Tema2::~Tema2() {
}

void Tema2::Init() {
    tank = new Tank(vec3(0.0f, -0.096694f, 0.0f), 0.0f, vec3(1.42955f, 0.735862f, 2.551433f), 1.4763052f, 5);
    speed = 2; // speed by default

    camera = new camera::Camera();
    camera->Set(cameraOffset, tank->position, up);
    cameraMatrix = perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    thirdPersonCamera = false;

    // Spawn 35 buildings
    vector<vec2> tankVertices = tank->GetVerticesWithPipe2D();
    for (int i = 0; i < 35; i++) {
        Building* building = new Building(tankVertices);
        buildings.push_back(building);
    }

    // Spawn 10 enemies
    for (int i = 0; i < 10; i++) {
        float x = (float) (rand() % 50 - 25);
        float z = (float) (rand() % 50 - 25);

        Tank* enemy = new Tank(vec3(0.0f, -0.096694f, 0.0f), 0.0f, vec3(1.42955f, 0.735862f, 2.551433f), 1.4763052f, 3);
        enemy->SpawnEnemy(groundCoordinates, buildings, tank, enemies);
        enemies.push_back(enemy);
    }

    Background::Init();
}

void Tema2::Update(float deltaTimeSeconds) {
    // Move tank
    tank->Move(deltaTimeSeconds, enemies, buildings);

    if (!thirdPersonCamera) {
        float sensivityOY = 0.001f;
        float position = (float) (window->GetResolution().x / 2 - window->GetCursorPosition().x);
        tank->Move(position, sensivityOY);
    }

    if(!thirdPersonCamera) {
        mat4 rotationMat = rotate(mat4(1), tank->angle, up);
        vec3 rotatedCameraOffset = vec3(rotationMat * vec4(cameraOffset, 1.0));
        camera->Set(tank->position + rotatedCameraOffset, tank->position, vec3(0, 1, 0));

        camera->RotateThirdPerson_OX(cameraAngle);
    }

    if (gameOver) {
        // Delete all bullets and enemies
        for (int i = 0; i < tank->bullets.size(); i++) {
            delete tank->bullets[i];
        }
        tank->bullets.clear();

        for (int i = 0; i < enemies.size(); i++) {
            for (int j = 0; j < enemies[i]->bullets.size(); j++) {
                delete enemies[i]->bullets[j];
            }
            enemies[i]->bullets.clear();
        }

        // Still render the background
        Background::Update(deltaTimeSeconds);
        return;
    }

    // Move enemies
    // for (int i = 0; i < enemies.size(); i++) {
    //     enemies[i]->Move(deltaTimeSeconds, enemies);
    // }

    // Move bullets and check collision
    for (int i = 0; i < tank->bullets.size(); i++) {
        // Check if bullet is still alive
        if (!tank->bullets[i]->isAlive) {
            score -= 1;
            tank->bullets.erase(tank->bullets.begin() + i, tank->bullets.begin() + i + 1);
            continue;
        }
        tank->bullets[i]->Move(deltaTimeSeconds);

        // Check collision with buildings
        for (int j = 0; j < buildings.size(); j++) {
            Building* building = buildings[j];
            vec2 bulletPosition = vec2(tank->bullets[i]->position.x, tank->bullets[i]->position.z);
            vector<vec2> buildingVertices = building->GetVertices2D();

            // Check if bullet is in building
            if (pointInRectangle(bulletPosition, buildingVertices)) {
                tank->bullets[i]->isAlive = false;
                continue;
            }
        }

        // Check collision with enemies
        for (int j = 0; j < enemies.size(); j++) {
            Tank* enemy = enemies[j];
            if (enemy->isAlive && tank->bullets[i]->isAlive) {
                // Check collision
                vec2 bulletPosition = vec2(tank->bullets[i]->position.x, tank->bullets[i]->position.z);
                vector<vec2> enemyVertices = enemy->GetVertices2D();

                // Check if bullet is in enemy
                if (pointInRectangle(bulletPosition, enemyVertices)) {
                    enemy->lives--;
                    if (enemy->lives == 0) {
                        enemy->isAlive = false;
                        score += 50;
                    }
                    tank->bullets[i]->isAlive = false;
                    break;
                }
            }
        }
    }

    // Rotate enemies and shoot
    for (int i = 0; i < enemies.size(); i++) {
        // Check if enemy is still alive
        if (!enemies[i]->isAlive) {
            enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
            continue;
        }
        enemies[i]->RotateAndShoot(tank, deltaTimeSeconds, buildings);

        // Move bullets and check collision
        for (int j = 0; j < enemies[i]->bullets.size(); j++) {
            // Check if bullet is still alive
            if (!enemies[i]->bullets[j]->isAlive) {
                enemies[i]->bullets.erase(enemies[i]->bullets.begin() + j, enemies[i]->bullets.begin() + j + 1);
                continue;
            }
            enemies[i]->bullets[j]->Move(deltaTimeSeconds);

            // Check collision with buildings
            for (int k = 0; k < buildings.size(); k++) {
                Building* building = buildings[k];
                vec2 bulletPosition = vec2(enemies[i]->bullets[j]->position.x, enemies[i]->bullets[j]->position.z);
                vector<vec2> buildingVertices = building->GetVertices2D();

                // Check if bullet is in building
                if (pointInRectangle(bulletPosition, buildingVertices)) {
                    enemies[i]->bullets[j]->isAlive = false;
                    break;
                }
            }

            // Check collision with tank
            if (tank->isAlive && enemies[i]->bullets[j]->isAlive) {
                // Check collision
                vec2 bulletPosition = vec2(enemies[i]->bullets[j]->position.x, enemies[i]->bullets[j]->position.z);
                vector<vec2> tankVertices = tank->GetVertices2D();

                // Check if bullet is in tank
                if (pointInRectangle(bulletPosition, tankVertices)) {
                    tank->lives--;
                    enemies[i]->bullets[j]->isAlive = false;
                    break;
                }
            }
        }
    }

    Background::Update(deltaTimeSeconds);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    float angleOffset = speed * deltaTime;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            tank->MoveFront(deltaTime, speed, groundCoordinates);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            tank->RotateLeft(deltaTime, speed, groundCoordinates);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            tank->MoveBack(deltaTime, speed, groundCoordinates);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            tank->RotateRight(deltaTime, speed, groundCoordinates);
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_R) {
        // Restart game
        tank->lives = 5;
        highest_score = std::max(highest_score, score);
        score = 50;
        start = chrono::steady_clock::now();
        gameOver = false;

        for (int i = 0; i < buildings.size(); i++) {
            delete buildings[i];
        }
        buildings.clear();

        for (int i = 0; i < enemies.size(); i++) {
            delete enemies[i];
        }
        enemies.clear();

        delete tank;

        Tema2::Init();
    }

    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
        speed = 5;
    }

    if (key == GLFW_KEY_ESCAPE) {
        // Exit game - Free memory
        for (int i = 0; i < buildings.size(); i++) {
            delete buildings[i];
        }

        for (int i = 0; i < enemies.size(); i++) {
            delete enemies[i];
        }

        delete tank;
        
        exit(0);
    }
}

void Tema2::OnKeyRelease(int key, int mods) {
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
        speed = 2;
    }
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        // Set the camera to rotate around the tank
        camera->RotateThirdPerson_OX(deltaY * sensivityOX);
        camera->RotateThirdPerson_OY(deltaX * sensivityOY);
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        thirdPersonCamera = true;
    }

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !gameOver) {
        // Shoot
        tank->Shoot();
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        thirdPersonCamera = false;
    }
}

float dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

bool pointInRectangle(vec2 point, vector<vec2> rectangleVertices) {
    // Find the vertice with max x
    float maxX = rectangleVertices[0].x;
    float minY = rectangleVertices[0].y;
    float maxY = rectangleVertices[0].y;
    vec2 A = rectangleVertices[0];
    vec2 B = rectangleVertices[0];
    vec2 C = rectangleVertices[0];

    for (int i = 1; i < rectangleVertices.size(); i++) {
        if (rectangleVertices[i].x > maxX) {
            maxX = rectangleVertices[i].x;
            B = rectangleVertices[i];
        }
        if (rectangleVertices[i].y < minY) {
            minY = rectangleVertices[i].y;
            A = rectangleVertices[i];
        }
        if (rectangleVertices[i].y > maxY) {
            maxY = rectangleVertices[i].y;
            C = rectangleVertices[i];
        }
    }

    vec2 AB = B - A;
    vec2 AM = point - A;
    vec2 BC = C - B;
    vec2 BM = point - B;
    float AB_AM = dot(AB, AM);
    float AB_AB = dot(AB, AB);
    float BC_BM = dot(BC, BM);
    float BC_BC = dot(BC, BC);

    return 0 <= AB_AM && AB_AM <= AB_AB && 0 <= BC_BM && BC_BM <= BC_BC;
}
