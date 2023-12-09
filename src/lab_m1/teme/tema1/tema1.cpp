#include "lab_m1/teme/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab4/transform3D.h"
#include "lab_m1/teme/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    // Load the scene
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 1));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initialize variables
    lives = 3;
    no_diamonds = 5;
    diamonds_general_cooldown = (rand() % 5 + 5) * 1.0f; // 5 - 10 seconds
    enemies_general_cooldown = (rand() % 3 + 7) * 1.0f; // 7 - 9 seconds
    diamond_cooldown = diamonds_general_cooldown;
    enemies_cooldown = enemies_general_cooldown;
    squareSide = 1.0f * (35 * resolution.x / 410); // square side according to the resolution
    rocket_selected = false; // no rocket selected

    // Load grid square
    Mesh* square_grid = create_object2D::CreateSquare("square_grid", glm::vec3(0, 0, 0), light_pink, squareSide, true);
    AddMeshToList(square_grid);

    // Load square box for the rockets
    Mesh* square_box = create_object2D::CreateSquare("square_box", glm::vec3(0, 0, 0), dark_gray, squareSide);
    AddMeshToList(square_box);

    // Load left bar
	glm::vec3 leftCorner = glm::vec3(squareSide / 20, squareSide / 5, 0);
	glm::vec3 rightCorner = glm::vec3(squareSide / 20 + squareSide / 2, squareSide / 5 + 3 * squareSide + 2 * squareSide / 5, 0);
	Mesh* bar = create_object2D::CreateRedBar("bar", leftCorner, rightCorner, true);
	AddMeshToList(bar);

    // Load the 4 types of rockets
    Mesh* rocket = create_object2D::CreateRocket("rocket1", glm::vec3(0, 0, 0), squareSide / 2, light_blue, true);
    AddMeshToList(rocket);
    rockets.push_back(Rocket { rocket, glm::vec3(0, 0, 0), squareSide, glm::mat3(1), 0, 1, false, false, false, -1, 0.5f});

    rocket = create_object2D::CreateRocket("rocket2", glm::vec3(0, 0, 0), squareSide / 2, light_red, true);
    AddMeshToList(rocket);
    rockets.push_back(Rocket { rocket, glm::vec3(0, 0, 0), squareSide, glm::mat3(1), 1, 2, false, false, false, -1, 0.5f});

    rocket = create_object2D::CreateRocket("rocket3", glm::vec3(0, 0, 0), squareSide / 2, light_green, true);
    AddMeshToList(rocket);
    rockets.push_back(Rocket { rocket, glm::vec3(0, 0, 0), squareSide, glm::mat3(1), 2, 2, false, false, false, -1, 0.5f});

    rocket = create_object2D::CreateRocket("rocket4", glm::vec3(0, 0, 0), squareSide / 2, light_orange, true);
    AddMeshToList(rocket);
    rockets.push_back(Rocket { rocket, glm::vec3(0, 0, 0), squareSide, glm::mat3(1), 3, 3, false, false, false, -1, 0.5f});

    // Load heart
    heart_radius = 0;
    Mesh* heart = create_object2D::CreateHeart("heart", glm::vec3(0, 0, 0), squareSide / 50, heart_radius, true);
    AddMeshToList(heart);

    // Load the 4 types of enemies
    Mesh* enemy = create_object2D::CreateHexagon("enemy1", glm::vec3(0, 0, 0), squareSide / 4, light_blue, true, true);
    AddMeshToList(enemy);

    enemy = create_object2D::CreateHexagon("enemy2", glm::vec3(0, 0, 0), squareSide / 4, light_red, true, true);
    AddMeshToList(enemy);

    enemy = create_object2D::CreateHexagon("enemy3", glm::vec3(0, 0, 0), squareSide / 4, light_green, true, true);
    AddMeshToList(enemy);

    enemy = create_object2D::CreateHexagon("enemy4", glm::vec3(0, 0, 0), squareSide / 4, light_orange, true, true);
    AddMeshToList(enemy);

    // Load a diamond
    diamond_radius = squareSide / 4;
    Mesh* diamond = create_object2D::CreateDiamondRing("mini_diamond", glm::vec3(0, 0, 0), diamond_radius / 2);
    AddMeshToList(diamond);

    diamond = create_object2D::CreateDiamondRing("diamond", glm::vec3(0, 0, 0), diamond_radius);
    AddMeshToList(diamond);

    // Load stars
    star_radius = 0;
    Mesh* star = create_object2D::CreateStar("star1", glm::vec3(0, 0, 0), squareSide / 2, star_radius, light_blue, true);
    AddMeshToList(star);

    star = create_object2D::CreateStar("star2", glm::vec3(0, 0, 0), squareSide / 2, star_radius, light_red, true);
    AddMeshToList(star);

    star = create_object2D::CreateStar("star3", glm::vec3(0, 0, 0), squareSide / 2, star_radius, light_green, true);
    AddMeshToList(star);

    star = create_object2D::CreateStar("star4", glm::vec3(0, 0, 0), squareSide / 2, star_radius, light_orange, true);
    AddMeshToList(star);

    // Create a shader program for background
    {
        Shader *shader = new Shader("BackgroundShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema1", "texture");
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "space.jpg").c_str(), GL_REPEAT);
        mapTextures["backgroundTexture"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "diamond.png").c_str(), GL_REPEAT);
        mapTextures["diamondTexture"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "over.png").c_str(), GL_REPEAT);
        mapTextures["GameOver"] = texture;
    }

    // Load background
    Mesh* background = create_object2D::CreateBackground("background", resolution);
    AddMeshToList(background);
}

// Decrease the lifes of the player and freeze the game if the player has no more lifes
// Return: bool - true if the player life was decreased, false otherwise
bool Tema1::DecreaseLifes(Enemy &enemy, int row, int pos) {
    enemy.modelMatrix *= transform2D::Scale(0.9f, 0.9f);
    enemy.radius *= 0.9f;
    if (enemy.radius < 1) {
        enemies[row].erase(enemies[row].begin() + pos);
        lives--;
        if (lives == 0) {
            // Print game over in GUI, not in console
            cout << "Game over!" << endl ;
            cout << "Your score is " << killed_enemies << "! (" << killed_enemies / 5 << " levels)" << endl;
            cout << "Press R to restart the game!" << endl;
            
            // freeze the game
            enemies_cooldown = 1000;
            diamond_cooldown = 1000;
            for (int i = 0; i < 3; i++) {
                enemies[i].erase(enemies[i].begin(), enemies[i].begin() + enemies[i].size());
            }
            diamonds.erase(diamonds.begin(), diamonds.begin() + diamonds.size());
        }
        return true;
    }
    return false;
}

void Tema1::RenderShaderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    float time = Engine::GetElapsedTime();
    glUniform1f(glGetUniformLocation(shader->program, "time"), time);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema1::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::DrawScene(float deltaTimeSeconds) {
    // Draw random diamonds
    for (int i = 0; i < diamonds.size(); i++) {
        RenderShaderMesh(meshes["diamond"], shaders["BackgroundShader"], diamonds[i].modelMatrix, mapTextures["diamondTexture"]);
    }

    // Draw selected rocket
    if (rocket_selected) {
        RenderMesh2D(selected_rocket.mesh, shaders["VertexColor"], selected_rocket.modelMatrix);
    }

    // Draw enemies
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < enemies[i].size(); j++) {
            if (enemies[i][j].no_lives == 0) {
                bool print = KillEnemy(enemies[i][j], i, j);
                if (print) {
                    string meshName = "enemy" + to_string(enemies[i][j].type + 1);
                    RenderMesh2D(meshes[meshName], shaders["VertexColor"], enemies[i][j].modelMatrix);
                }
                continue;
            }
            enemies[i][j].modelMatrix *= transform2D::Translate(-enemies[i][j].translateX, 0);
            enemies[i][j].center.x -= enemies[i][j].translateX;

            // Check if the enemy has reached the left bar
            bool decrease = false;
            if (enemies[i][j].center.x - enemies[i][j].radius < squareSide / 20 + squareSide / 4) {
                if (DecreaseLifes(enemies[i][j], i, j)) {
                    continue;
                }
            }

            // Check if the enemy has reached a rocket
            for (int k = 0; k < 3; k++) {
                if (rockets_grid[i][k].is_on_grid) {
                    // Check if the enemy has reached the rocket
                    if (enemies[i][j].center.x - enemies[i][j].radius < rockets_grid[i][k].center.x + rockets_grid[i][k].side / 2 &&
                        enemies[i][j].center.x + enemies[i][j].radius > rockets_grid[i][k].center.x - rockets_grid[i][k].side / 2) {
                        rockets_grid[i][k].is_destroyed = true;
                    }
                }
            }

            string meshName = "enemy" + to_string(enemies[i][j].type + 1);
            RenderMesh2D(meshes[meshName], shaders["VertexColor"], enemies[i][j].modelMatrix);
        }
    }

    // Draw the 4 rockets grid
    for (int i = 0; i < 4; i++) {
        float next_square_x = (i + 1) * (squareSide / 2) + i * squareSide;
        float next_square_y = resolution.y - squareSide - squareSide / 10;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(next_square_x, next_square_y);
        RenderMesh2D(meshes["square_box"], shaders["VertexColor"], modelMatrix);

        modelMatrix *= transform2D::Translate(squareSide / 2, squareSide / 2);
        string meshName = "rocket" + to_string(i + 1);
        RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);

        rockets[i].center = glm::vec3(next_square_x + squareSide / 2, next_square_y + squareSide / 2, 0);

        glm::mat4 matrix = glm::mat4(1);
        matrix *= transform3D::Translate(next_square_x + squareSide / 2, next_square_y + squareSide / 2, 0);
        matrix *= transform3D::Translate(diamond_radius / 2, - diamond_radius / 2, 0);
        matrix *= transform3D::Translate(- squareSide / 2, - squareSide / 2, 0);
        matrix *= transform3D::Scale(0.75f, 0.75f, 0);

        switch (i) {
            case 0:
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                break;
            case 1:
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                matrix *= transform3D::Translate(2.5f * diamond_radius / 2, 0, 0);
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                break;
            case 2:
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                matrix *= transform3D::Translate(2.5f * diamond_radius / 2, 0, 0);
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                break;
            case 3:
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                matrix *= transform3D::Translate(2.5f * diamond_radius / 2, 0, 0);
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                matrix *= transform3D::Translate(2.5f * diamond_radius / 2, 0, 0);
                RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
                break;
            
            default:
                break;
        }
    }

    // Draw lives
    for (int i = 0; i < lives; i++) {
        float next_square_x = (i + 1) * squareSide + i * heart_radius + 6 * squareSide;
        float next_square_y = resolution.y - squareSide / 2 - squareSide / 10;

        heart_modelMatrix = glm::mat3(1);
        heart_modelMatrix *= transform2D::Translate(next_square_x, next_square_y);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], heart_modelMatrix);
    }

    // Draw diamond bar
    float next_x = squareSide / 2 + 6 * squareSide;
    float next_y = resolution.y - squareSide / 2 - heart_radius - squareSide / 10;

    glm::mat4 matrix = glm::mat4(1);
    matrix *= transform3D::Translate(next_x + diamond_radius / 2, next_y - diamond_radius / 2, 0);
    matrix *= transform3D::Scale(0.75f, 0.75f, 0);
    for (int i = 0; i < no_diamonds; i++) {
        RenderShaderMesh(meshes["mini_diamond"], shaders["BackgroundShader"], matrix, mapTextures["diamondTexture"]);
        matrix *= transform3D::Translate(2.5f * diamond_radius / 2, 0, 0);
    }

    // Draw left bar
    modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);

    // Draw stars
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < stars[i].size(); j++) {
            stars[i][j].center.x += stars[i][j].translateX;
            stars[i][j].center.y += stars[i][j].translateY;
            // Move the star to the right and rotate it
            stars[i][j].modelMatrix *= transform2D::Translate(stars[i][j].translateX, stars[i][j].translateY);
            stars[i][j].angle_rotate += deltaTimeSeconds;

            glm::mat3 modelMatrix = transform2D::Rotate(-stars[i][j].angle_rotate);

            // Check if the star is still on the screen
            if (stars[i][j].center.x + star_radius > resolution.x) {
                stars[i].erase(stars[i].begin() + j);
                break;
            }

            // Check if the star has reached an enemy on its row on its type
            bool erase = CheckIfHit(stars[i][j], i);
            if (erase) {
                stars[i].erase(stars[i].begin() + j);
                break;
            } else {
                string meshName = "star" + to_string(stars[i][j].type + 1);
                RenderMesh2D(meshes[meshName], shaders["VertexColor"], stars[i][j].modelMatrix * modelMatrix);
            }
        }
    }

    // Draw 3 x 3 grid with the rockets if they are bought
	float padding = squareSide / 5 + squareSide / 2;
	float distance_squares = squareSide / 5;

	for (int i = 0; i < 9; i++) {
		next_x = padding + 1.0f *(i % 3) * (squareSide + distance_squares);
		next_y = distance_squares + (i / 3) * (squareSide + distance_squares);
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(next_x, next_y);

        if (rockets_grid[i / 3][i % 3].is_on_grid) {
            if (rockets_grid[i / 3][i % 3].is_destroyed) {
                if (RemoveRocket(rockets_grid[i / 3][i % 3])) {
                    RenderMesh2D(meshes["square_grid"], shaders["VertexColor"], modelMatrix);
                    continue;
                }
            }

            ShootIfPossible(rockets_grid[i / 3][i % 3], deltaTimeSeconds);

            string meshName = "rocket" + to_string(rockets_grid[i / 3][i % 3].type + 1);
            RenderMesh2D(meshes[meshName], shaders["VertexColor"], rockets_grid[i / 3][i % 3].modelMatrix);
        }

		RenderMesh2D(meshes["square_grid"], shaders["VertexColor"], modelMatrix);
	}

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderShaderMesh(meshes["background"], shaders["BackgroundShader"], modelMatrix, mapTextures["backgroundTexture"]);
    }
}

bool Tema1::KillEnemy(Enemy &enemy, int row, int pos) {
    enemy.modelMatrix *= transform2D::Scale(0.9f, 0.9f);
    enemy.radius *= 0.9f;
    if (enemy.radius < 1) {
        enemies[row].erase(enemies[row].begin() + pos);
        killed_enemies++;

        if (killed_enemies % 5 == 0 && killed_enemies != 0) {
            cout << "Level up! Your level is " << killed_enemies / 5 << endl;
            // Increase the number of diamonds that appear on the screen
            diamonds_general_cooldown += 0.5f;
            // Increase the number of enemies that appear on the screen
            enemies_general_cooldown -= 0.5f;
        }

        return false;
    }

    return true;
}

bool Tema1::CheckIfHit(Star &star, int row) {
    for (int i = 0; i < enemies[row].size(); i++) {
        if (enemies[row][i].type == star.type) {
            // Check if the star has reached the enemy
            if (star.center.x + star_radius > enemies[row][i].center.x - enemies[row][i].radius) {
                enemies[row][i].no_lives--;
                return true;
            }
        }
    }
    return false;
}

void Tema1::ShootIfPossible(Rocket &rocket, float deltaTimeSeconds) {
    if (rocket.stars_cooldown > 0) {
        rocket.stars_cooldown -= deltaTimeSeconds;
    }
    
    // Check if the rocket has an enemy on its row
    if (enemies[rocket.row].size() > 0) {
        // Check if the rocket has an enemy on its type and in front of it
        for (int i = 0; i < enemies[rocket.row].size(); i++) {
            if (enemies[rocket.row][i].type == rocket.type &&
                enemies[rocket.row][i].center.x > rocket.center.x) {
                // Shoot the enemy if the stars cooldown has passed
                if (rocket.stars_cooldown < 0) {
                    rocket.stars_cooldown = 0.5f;
                    float next_x = rocket.center.x + rocket.side / 2;
                    float next_y = rocket.center.y;

                    Star star;
                    star.translateX = squareSide / 16;
                    star.translateY = 0;
                    star.angle_rotate = RADIANS(45);
                    star.center = glm::vec3(next_x, next_y, 0);
                    star.type = rocket.type;
                    star.modelMatrix = glm::mat3(1);
                    star.modelMatrix *= transform2D::Translate(next_x, next_y);

                    stars[rocket.row].push_back(star);
                }

                return;
            }
        }
    }
}

bool Tema1::RemoveRocket(Rocket &rocket) {
    // Scale the rocket until it becomes invisible
    rocket.modelMatrix *= transform2D::Scale(0.9f, 0.9f);
    rocket.side *= 0.9f;
    // If the rocket is invisible, remove it from the grid
    if (rocket.side < 1) {
        rocket = Rocket { NULL, glm::vec3(0, 0, 0), 0, glm::mat3(1), 0, 0, false, false, false, -1};
        return true;
    }
    return false;
}

void Tema1::Update(float deltaTimeSeconds) {
    DrawScene(deltaTimeSeconds);

    diamond_cooldown -= deltaTimeSeconds;
    // Draw 3 random diamonds on the screen at random time intervals and positions
    if (diamond_cooldown < 0) {
        diamond_cooldown = diamonds_general_cooldown;
        
        for (int i = 0; i < 3; i++) {
            float next_x = (rand() % (int) resolution.x) * 1.0f;
            float next_y = (rand() % (int) resolution.y) * 1.0f;

            // Check if the diamond is not drawn on the grid
            next_x += next_x < diamond_radius ? diamond_radius : 0;
            next_x -= next_x > resolution.x - diamond_radius ? diamond_radius : 0;

            next_y += next_y < diamond_radius ? diamond_radius : 0;
            next_y -= next_y > resolution.y - diamond_radius ? diamond_radius : 0;

            Diamond diamond;
            diamond.center = glm::vec3(next_x, next_y, 0);
            diamond.radius = diamond_radius;
            diamond.modelMatrix = glm::mat4(1);
            diamond.modelMatrix *= transform3D::Translate(next_x, next_y, 0);

            diamonds.push_back(diamond);
        }

        // Set the timer to 5 seconds
        diamond_time_elapsed = 5;
    }

    diamond_time_elapsed -= deltaTimeSeconds;
    // Remove the diamonds that are not collected in 5 seconds
    if (diamond_time_elapsed < 0) {
        if (diamonds.size() > 0) {
            diamonds.erase(diamonds.begin(), diamonds.begin() + diamonds.size());
        }
        diamond_time_elapsed = 5;
    }

    enemies_cooldown -= deltaTimeSeconds;
    // Draw random enemies at random time intervals and rows
    if (enemies_cooldown < 0) {
        enemies_cooldown = enemies_general_cooldown;

        int row = rand() % 3;
        // Set enemy at the end of the row
        float next_square_y = (row + 1) * squareSide / 5 + row * squareSide + squareSide / 2;
        float next_square_x = resolution.x - squareSide / 4;
 
        Enemy enemy;
        enemy.translateX = squareSide / 64;
        enemy.translateY = 0;
        enemy.center = glm::vec3(next_square_x, next_square_y, 0);
        enemy.radius = squareSide / 4;
        enemy.type = rand() % 4;
        enemy.modelMatrix = glm::mat3(1);
        enemy.modelMatrix *= transform2D::Translate(next_square_x, next_square_y);
        enemy.no_lives = 3;

        enemies[row].push_back(enemy);
    }

    // Drag and drop rockets
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && rocket_selected) {
        // Check if the mouse is on a rocket
        glm::ivec2 cursor = window->GetCursorPosition();
        float mouseX = 1.0f * cursor.x;
        float mouseY = 1.0f * resolution.y - cursor.y;
        selected_rocket.modelMatrix = glm::mat3(1);
        selected_rocket.modelMatrix *= transform2D::Translate(mouseX, mouseY);
        selected_rocket.center = glm::vec3(mouseX, mouseY, 0);
    }
}

void Tema1::FrameEnd() {
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {
}

void Tema1::OnKeyPress(int key, int mods) {
    if(key == GLFW_KEY_R) {
        cout << "Restarting the game!" << endl;
        // Reload the game
        lives = 3;
        no_diamonds = 5;
        diamonds_general_cooldown = (rand() % 5 + 5) * 1.0f; // 5 - 10 seconds
        enemies_general_cooldown = (rand() % 3 + 7) * 1.0f; // 7 - 9 seconds
        diamond_cooldown = diamonds_general_cooldown;
        enemies_cooldown = enemies_general_cooldown;
        rocket_selected = false;
        killed_enemies = 0;

        for (int i = 0; i < 3; i++) {
            enemies[i].erase(enemies[i].begin(), enemies[i].begin() + enemies[i].size());
            stars[i].erase(stars[i].begin(), stars[i].begin() + stars[i].size());
            
            for (int j = 0; j < 3; j++) {
                rockets_grid[i][j] = Rocket { NULL, glm::vec3(0, 0, 0), 0, glm::mat3(1), 0, 0, false, false, false, -1};
            }
        }
    }
}

void Tema1::OnKeyRelease(int key, int mods) {
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    mouseY = resolution.y - mouseY;

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        // if you press the left button on a diamond, colect it
        for (int i = 0; i < diamonds.size(); i++) {
            // Check if the mouse is on the diamond
            double distance = sqrt(pow(diamonds[i].center.x - mouseX, 2) + pow(diamonds[i].center.y - mouseY, 2));
            if (distance < diamonds[i].radius) {
                diamonds.erase(diamonds.begin() + i);
                no_diamonds++;
                break;
            }
        }
    }

    // if you press the left button on a rocket, select it to drag and drop it
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < 4; i++) {
            if (rockets[i].center.x - rockets[i].side / 2 < mouseX && mouseX < rockets[i].center.x + rockets[i].side / 2) {
                if (rockets[i].center.y - rockets[i].side / 2 < mouseY && mouseY < rockets[i].center.y + rockets[i].side / 2) {
                    // Check if you have enough diamonds to buy the rocket
                    if (no_diamonds >= rockets[i].no_diamonds) {
                        // select the rocket
                        rocket_selected = true;
                        selected_rocket = rockets[i];
                        selected_rocket.center = glm::vec3(mouseX, mouseY, 0);
                        selected_rocket.modelMatrix = glm::mat3(1);
                        selected_rocket.modelMatrix *= transform2D::Translate(1.0f * mouseX, 1.0f * mouseY);
                    }
                }
            }
        }
    }

    // Scale the clicked rocket until it becomes invisible and remove it from the grid
    // if you press the right button on a rocket that is on the grid, remove it
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        // Check if the mouse is on a grid square
        float padding = squareSide / 5 + squareSide / 2;
        float distance_squares = squareSide / 5;

        for (int i = 0; i < 9; i++) {
            // Coordinates of the center of the square
            float next_x = padding + 1.0f * (i % 3) * (squareSide + distance_squares) + squareSide / 2;
            float next_y = distance_squares + (i / 3) * (squareSide + distance_squares) + squareSide / 2;

            // Check if the mouse is on the square
            if (next_x - squareSide / 2 < mouseX && mouseX < next_x + squareSide / 2) {
                if (next_y - squareSide / 2 < mouseY && mouseY < next_y + squareSide / 2) {
                    // Check if the square is not empty
                    if (rockets_grid[i / 3][i % 3].is_on_grid) {
                        rockets_grid[i / 3][i % 3].is_destroyed = true;
                        break;
                    }
                }
            }
        }
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
    mouseY = resolution.y - mouseY;

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && rocket_selected) {
        // Deselect the rocket
        rocket_selected = false;

        // Check if the mouse is on a grid square
        float padding = squareSide / 5 + squareSide / 2;
        float distance_squares = squareSide / 5;

        for (int i = 0; i < 9; i++) {
            // Coordinates of the center of the square
            float next_x = padding + 1.0f * (i % 3) * (squareSide + distance_squares) + squareSide / 2;
            float next_y = distance_squares + (i / 3) * (squareSide + distance_squares) + squareSide / 2;

            // Check if the mouse is on the square
            if (next_x - squareSide / 2 < mouseX && mouseX < next_x + squareSide / 2) {
                if (next_y - squareSide / 2 < mouseY && mouseY < next_y + squareSide / 2) {
                    // Check if the square is empty
                    if (!rockets_grid[i / 3][i % 3].is_on_grid) {
                        // Check if you have enough diamonds to buy the rocket
                        if (no_diamonds >= selected_rocket.no_diamonds) {
                            rockets_grid[i / 3][i % 3] = selected_rocket;
                            rockets_grid[i / 3][i % 3].is_on_grid = true;
                            rockets_grid[i / 3][i % 3].center = glm::vec3(next_x, next_y, 0);
                            rockets_grid[i / 3][i % 3].modelMatrix = glm::mat3(1);
                            rockets_grid[i / 3][i % 3].modelMatrix *= transform2D::Translate(next_x, next_y);
                            rockets_grid[i / 3][i % 3].row = i / 3;
                            rockets_grid[i / 3][i % 3].stars_cooldown = 0.5f;
                            no_diamonds -= selected_rocket.no_diamonds;
                        }
                    }
                }
            }
        }
    }
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema1::OnWindowResize(int width, int height) {
}
