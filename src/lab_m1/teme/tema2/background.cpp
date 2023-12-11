#include "lab_m1/teme/tema2/headers/background.h"

using namespace std;
using namespace m1;
using namespace glm;

Background::Background() {
}


Background::~Background() {
}


void Background::Init() {
    // Load ground
    {
        Mesh* mesh = create_object3D::CreateGround("ground", vec2(1, 1));
        AddMeshToList(mesh);
    }

    // Load trees
    const string sourceTreeDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "objects/tree");
    {
        Mesh* mesh = new Mesh("tree-1");
        mesh->LoadMesh(sourceTreeDir, "tree_1.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tree-2");
        mesh->LoadMesh(sourceTreeDir, "tree_2.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tree-3");
        mesh->LoadMesh(sourceTreeDir, "tree_3.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load tank parts
    const string sourceTanktDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "objects/tank");
    {
        Mesh* mesh = new Mesh("baza");
        mesh->LoadMesh(sourceTanktDir, "baza.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("sine");
        mesh->LoadMesh(sourceTanktDir, "sine.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("teava");
        mesh->LoadMesh(sourceTanktDir, "teava.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("turela");
        mesh->LoadMesh(sourceTanktDir, "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load enemy parts
    const string sourceEnemyDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "objects/enemy");
    {
        Mesh* mesh = new Mesh("bazaEnemy");
        mesh->LoadMesh(sourceEnemyDir, "baza.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("turelaEnemy");
        mesh->LoadMesh(sourceEnemyDir, "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load bullet
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create shader programs
    {
        Shader *shader = new Shader("NormalShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

        shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "VertexTank.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "FragmentTank.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

        shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "VertexEnemy.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "shaders", "FragmentEnemy.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load ground texture
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "teme/tema2", "texture");
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_with_flowers.png").c_str(), GL_REPEAT);
        mapTextures["grassTexture"] = texture;
    }

    // Load stone texture
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.png").c_str(), GL_REPEAT);
        mapTextures["stoneTexture"] = texture;
    }

    // Load bullet texture
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "metal.png").c_str(), GL_REPEAT);
        mapTextures["bulletTexture"] = texture;
    }

    // Load text
    {
        ivec2 resolution = window->GetResolution();
        smallText = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
        smallText->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);

        bigText = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
        bigText->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 72);
    }

}

void Background::FrameStart() {
    // Clears the color buffer, making the background dark blue
    glClearColor(0, 0, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Background::Update(float deltaTimeSeconds) {
    // Render just the faces that are not turned away from the camera
    // glEnable(GL_CULL_FACE);

    // Render bullets
    for (int i = 0; i < tank->bullets.size(); i++) {
        Bullet* bullet = tank->bullets[i];
        RenderShaderMesh(meshes["sphere"], shaders["NormalShader"], bullet->modelMatrix, mapTextures["bulletTexture"]);
    }

    // Render enemies bullets
    for (int i = 0; i < enemies.size(); i++) {
        Tank* enemy = enemies[i];
        for (int j = 0; j < enemy->bullets.size(); j++) {
            Bullet* bullet = enemy->bullets[j];
            RenderShaderMesh(meshes["sphere"], shaders["NormalShader"], bullet->modelMatrix, mapTextures["bulletTexture"]);
        }
    }

    // Render tank
    {
        RenderMesh(meshes["baza"], shaders["TankShader"], tank->modelMatrix, NULL);
        RenderMesh(meshes["sine"], shaders["TankShader"], tank->modelMatrix, NULL);
        RenderMesh(meshes["teava"], shaders["TankShader"], tank->pipe->modelMatrix, NULL);
        RenderMesh(meshes["turela"], shaders["TankShader"], tank->turela->modelMatrix, NULL);
    }

    // Render enemies
    for (int i = 0; i < enemies.size(); i++) {
        Tank* enemy = enemies[i];
        if (enemy->isAlive) {
            RenderMesh(meshes["bazaEnemy"], shaders["EnemyShader"], enemy->modelMatrix, enemy);
            RenderMesh(meshes["sine"], shaders["EnemyShader"], enemy->modelMatrix, enemy);
            RenderMesh(meshes["teava"], shaders["EnemyShader"], enemy->pipe->modelMatrix, enemy);
            RenderMesh(meshes["turelaEnemy"], shaders["EnemyShader"], enemy->turela->modelMatrix, enemy);
        }
    }

    // Render trees
    for (int i = 0; i < buildings.size(); i++) {
        Building* building = buildings[i];
        mat4 modelMatrix = translate(mat4(1), building->position);
        RenderMesh(meshes["tree-" + to_string(building->buildingType)], shaders["NormalShader"], modelMatrix, NULL);
    }

    // Render ground
    {
        mat4 modelMatrix = mat4(1);
        for (int offset_x = -25; offset_x < 25; offset_x++) {
            for (int offset_z = -25; offset_z < 25; offset_z++) {
                modelMatrix = translate(mat4(1), vec3(offset_x, 0, offset_z));
                RenderShaderMesh(meshes["ground"], shaders["NormalShader"], modelMatrix, mapTextures["grassTexture"]);
            }
        }
    }
}

void Background::DrawHUD() {
    if (!gameOver) {
        end = chrono::steady_clock::now();
        elapsed_seconds = end - start;
    }

    const float kTopY = 10.f;
    const float kRowHeight = 25.f;

    int rowIndex = 0;
    string endGameMessage = "";
    string timeElapsed = "Time elapsed: " + to_string((int)elapsed_seconds.count()) + " seconds";
    string scoreMessage = "Score: " + to_string(score);
    string livesMessage = "Lives: " + to_string(tank->lives);
    string highestScoreMessage = "Highest score: " + to_string(highest_score);
    string exitMessage = "Press Esc to exit the game.";
    string restartMessage = "";
    if (elapsed_seconds.count() > 100) {
        endGameMessage = "Game over!";
        timeElapsed = "Time elapsed: 100 seconds";
        restartMessage = "Press R to restart";
    }
    if (tank->lives <= 0 || score < 0) {
        endGameMessage = "Game over!";
        restartMessage = "Press R to restart";
    }
    if (enemies.size() == 0) {
        endGameMessage = "You won! Well done!";
        restartMessage = "Press R to restart";
    }

    smallText->RenderText(timeElapsed, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    smallText->RenderText(scoreMessage, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    smallText->RenderText(livesMessage, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    smallText->RenderText(exitMessage, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    smallText->RenderText(highestScoreMessage, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);

    if (endGameMessage != "") {
        float textX = 1.0f * (window->GetResolution().x) / 2;
        float textY = 1.0f * (window->GetResolution().y) / 2;
        bigText->RenderText(endGameMessage,textX - textX / 3, textY - 50, 1.0f, kTextColor);
        bigText->RenderText(restartMessage, textX - 2 * textX / 3, textY + 50, 1.0f, kTextColor);

        gameOver = true;
    }
}

void Background::FrameEnd() {
    DrawHUD();
    DrawCoordinateSystem(camera->GetViewMatrix(), cameraMatrix);
}

void Background::RenderShaderMesh(Mesh *mesh, Shader *shader, const mat4 & modelMatrix, Texture2D* texture) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, value_ptr(modelMatrix));

    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));

    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, value_ptr(cameraMatrix));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Background::RenderMesh(Mesh * mesh, Shader * shader, const mat4 & modelMatrix, Tank* tank) {
    if (!mesh  || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, value_ptr(cameraMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, value_ptr(modelMatrix));

    // If mesh is enemy, add hp uniform
    if (tank != NULL) {
        float hp = tank->lives / 3.0f;
        glUniform1f(glGetUniformLocation(shader->program, "hp"), hp);
    }

    mesh->Render();
}
