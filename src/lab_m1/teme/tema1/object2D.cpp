#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* create_object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    glm::vec3 color,
    float length,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* create_object2D::CreateRedBar(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    glm::vec3 rightUpperCorner,
    bool fill) {
    // set the color to red
    glm::vec3 color = glm::vec3(0.3411f, 0.0470f, 0.1372f);

    std::vector<VertexFormat> vertices = {
        VertexFormat(leftBottomCorner, color),
        VertexFormat(glm::vec3(rightUpperCorner.x, leftBottomCorner.y, 0), color),
        VertexFormat(rightUpperCorner, color),
        VertexFormat(glm::vec3(leftBottomCorner.x, rightUpperCorner.y, 0), color)
    };

    Mesh* bar = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        bar->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    bar->InitFromData(vertices, indices);
    return bar;
}

Mesh* create_object2D::CreatePentagon(
    const std::string &name,
    glm::vec3 center,
    float radius,
    int no_sides,
    glm::vec3 color,
    bool fill,
    bool doubleFill) {
    int vCount = no_sides;
    float angle = 360.0f / vCount;
    int triangleCount = vCount - 1;

    // The color of the outer polygon
    glm::vec3 color2 = color * 0.5f;

    std::vector<glm::vec3> temp;
    // Calculate positions of vertices
    for (int i = 0; i < vCount; i++) {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }
    // Create triangles
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < triangleCount; i++) {
        vertices.push_back(VertexFormat(center, color));
        vertices.push_back(VertexFormat(temp[i], color));
        vertices.push_back(VertexFormat(temp[i + 1], color));

        if (doubleFill) {
            vertices.push_back(VertexFormat(center, color2));
            vertices.push_back(VertexFormat(temp[i] * 1.25f, color2));
            vertices.push_back(VertexFormat(temp[i + 1] * 1.25f, color2));
        }
    }

    // Last triangle
    vertices.push_back(VertexFormat(temp[triangleCount], color));
    vertices.push_back(VertexFormat(temp[0], color));
    vertices.push_back(VertexFormat(center, color));

    if (doubleFill) {
        vertices.push_back(VertexFormat(temp[triangleCount] * 1.25f, color2));
        vertices.push_back(VertexFormat(temp[0] * 1.25f, color2));
        vertices.push_back(VertexFormat(center, color2));
    }

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    } else {
        for (int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* create_object2D::CreateCircle(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill,
    bool doubleFill) {

    return CreatePentagon(name, center, radius, 500, color, fill, doubleFill);
}

Mesh* create_object2D::CreateHexagon(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill,
    bool doubleFill) {

    return CreatePentagon(name, center, radius, 6, color, fill, doubleFill);
}

Mesh* create_object2D::CreateHeart(
    const std::string &name,
    glm::vec3 center,
    float radius,
    float &return_radius,
    bool fill) {
    int vCount = 1000;
    float angle = 360.0f / vCount;
    int triangleCount = vCount - 1;

    // The color of the heart is pink
    glm::vec3 color = glm::vec3(1, 0.4117f, 0.7058f);

    std::vector<glm::vec3> temp;
    // Calculate positions of vertices
    for (int i = 0; i < vCount; i++) {
        float currentAngleRad = glm::radians(angle * i);
        float x = radius * 16 * powf(sin(currentAngleRad), 3);
        float y = radius * (13 * cos(currentAngleRad) - 5 * cos(2 * currentAngleRad) - 2 * cos(3 * currentAngleRad) - cos(4 * currentAngleRad));
        float z = 0.0f;

        return_radius = MAX(return_radius, abs(x));
        return_radius = MAX(return_radius, abs(y));

        temp.push_back(glm::vec3(x, y, z));
    }

    // Create triangles
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < triangleCount; i++) {
        vertices.push_back(VertexFormat(center,color));
        vertices.push_back(VertexFormat(temp[i], color));
        vertices.push_back(VertexFormat(temp[i + 1], color));
    }

    // Last triangle
    vertices.push_back(VertexFormat(temp[triangleCount], color));
    vertices.push_back(VertexFormat(temp[0], color));
    vertices.push_back(VertexFormat(center, color));

    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i <= triangleCount * 3; i++) {
        indices.push_back(i);
    }

    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    } else {
        for (int i = 0; i < triangleCount * 3; i++) {
            indices.push_back(i);
        }
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

Mesh* create_object2D::CreateStar(
    const std::string &name,
    glm::vec3 center,
    float radius,
    float &return_radius,
    glm::vec3 color,
    bool fill) {
    int no_points = 2000;
    float angle = 2 * 360.0f / no_points;
    float a = 2.5f;
    std::vector<glm::vec3> temp;
    // Calculate positions of vertices
    for (int i = 0; i < no_points; i++) {
        float currentAngle = angle * i;
        float modulo = currentAngle / (2 * M_PI / a) - (int)(currentAngle / (2 * M_PI / a));
        float r = radius * (1 / cos(modulo - M_PI / a)) / 10;

        float x = r * cos(currentAngle);
        float y = r * sin(currentAngle);
        float z = 0.0f;

        return_radius = MAX(return_radius, abs(x));
        return_radius = MAX(return_radius, abs(y));

        temp.push_back(glm::vec3(x, y, z));
    }

    // Create triangles
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < no_points - 1; i++) {
        vertices.push_back(VertexFormat(center, color));
        vertices.push_back(VertexFormat(temp[i], color));
        vertices.push_back(VertexFormat(temp[i + 1], color));
    }

    // Last triangle
    vertices.push_back(VertexFormat(temp[no_points - 1], color));
    vertices.push_back(VertexFormat(temp[0], color));
    vertices.push_back(VertexFormat(center, color));

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i < no_points * 3; i++) {
        indices.push_back(i);
    }

    if (!fill) {
        star->SetDrawMode(GL_LINE_LOOP);
    } else {
        for (int i = 0; i < no_points * 3; i++) {
            indices.push_back(i);
        }
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* create_object2D::CreateRocket(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill) {
    // Initialize colors
    glm::vec3 light_gray = glm::vec3(0.6588f, 0.6588f, 0.6588f);
    glm::vec3 yellow = glm::vec3(1, 1, 0.4117f);

    // Create triangles
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(glm::vec3(center.x - radius, center.y + 3 * radius / 8, 0), color),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y + 3 * radius / 8, 0), color),

        VertexFormat(center, yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y + 3 * radius / 8, 0), yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y + 5 * radius / 8, 0), yellow),

        VertexFormat(center, yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y + 5 * radius / 8, 0), yellow),
        VertexFormat(glm::vec3(center.x - 3 * radius / 8, center.y + 3 * radius / 8, 0), yellow),

        VertexFormat(center, color),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y + 3 * radius / 8, 0), color),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y - 3 * radius / 8, 0), color),

        VertexFormat(center, light_gray),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y + 3 * radius / 8, 0), light_gray),
        VertexFormat(glm::vec3(center.x + radius, center.y, 0), light_gray),

        VertexFormat(center, light_gray),
        VertexFormat(glm::vec3(center.x + radius, center.y, 0), light_gray),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y - 3 * radius / 8, 0), light_gray),

        VertexFormat(center, color),
        VertexFormat(glm::vec3(center.x + radius / 4, center.y - 3 * radius / 8, 0), color),
        VertexFormat(glm::vec3(center.x - radius, center.y - 3 * radius / 8, 0), color),

        VertexFormat(center, yellow),
        VertexFormat(glm::vec3(center.x - 3 * radius / 8, center.y - 3 * radius / 8, 0), yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y - 5 * radius / 8, 0), yellow),

        VertexFormat(center, yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y - 5 * radius / 8, 0), yellow),
        VertexFormat(glm::vec3(center.x - radius, center.y - 3 * radius / 8, 0), yellow),


        VertexFormat(glm::vec3(center.x - radius, center.y - 3 * radius / 8, 0), color),
        VertexFormat(glm::vec3(center.x - radius, center.y + 3 * radius / 8, 0), color),
        VertexFormat(center, color)
    };

    Mesh* rocket = new Mesh(name);

    std::vector<unsigned int> indices;
    for (int i = 0; i < 30; i++) {
        indices.push_back(i);
    }

    if (!fill) {
        rocket->SetDrawMode(GL_LINE_LOOP);
    } else {
        for (int i = 0; i < 30; i++) {
            indices.push_back(i);
        }
    }

    rocket->InitFromData(vertices, indices);
    return rocket;
}

Mesh* create_object2D::CreateDiamondRing(const std::string &name, glm::vec3 center, float radius) {
    // glm::vec3 darker_color = color * 0.5f;
    // glm::vec3 lighter_color = color * 1.5f;

    // Create triangles
    std::vector<glm::vec3> positions = {
        glm::vec3(center.x, center.y - radius, 0),
        glm::vec3(center.x + radius, center.y, 0),
        glm::vec3(center.x + radius / 2, center.y + radius / 2, 0),
        
        glm::vec3(center.x, center.y - radius, 0),
        glm::vec3(center.x + radius / 2, center.y + radius / 2, 0),
        glm::vec3(center.x - radius / 2, center.y + radius / 2, 0),

        glm::vec3(center.x, center.y - radius, 0),
        glm::vec3(center.x - radius / 2, center.y + radius / 2, 0),
        glm::vec3(center.x - radius, center.y, 0)
    };

     std::vector<glm::vec3> normals {
        glm::vec3(0.5f, 0, 0.5f - 0.5f),
        glm::vec3(0.5f + 0.5f, 0, 0.5f),
        glm::vec3(0.5f + 0.5f / 2, 0, 0.5f + 0.5f / 2),
        
        glm::vec3(0.5f, 0, 0.5f - 0.5f),
        glm::vec3(0.5f + 0.5f / 2, 0, 0.5f + 0.5f / 2),
        glm::vec3(0.5f - 0.5f / 2, 0, 0.5f + 0.5f / 2),

        glm::vec3(0.5f, 0, 0.5f - 0.5f),
        glm::vec3(0.5f - 0.5f / 2, 0, 0.5f + 0.5f / 2),
        glm::vec3(0.5f - 0.5f, 0, 0.5f)
    };

    std::vector<glm::vec2> texCoords = {
        glm::vec2(0.5f, 0.5f - 0.5f),
        glm::vec2(0.5f + 0.5f, 0.5f),
        glm::vec2(0.5f + 0.5f / 2, 0.5f + 0.5f / 2),
        
        glm::vec2(0.5f, 0.5f - 0.5f),
        glm::vec2(0.5f + 0.5f / 2, 0.5f + 0.5f / 2),
        glm::vec2(0.5f - 0.5f / 2, 0.5f + 0.5f / 2),

        glm::vec2(0.5f, 0.5f - 0.5f),
        glm::vec2(0.5f - 0.5f / 2, 0.5f + 0.5f / 2),
        glm::vec2(0.5f - 0.5f, 0.5f)
    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i < 9; i++) {
        indices.push_back(i);
    }

    for (int i = 0; i < 9; i++) {
        indices.push_back(i);
    }

    diamond->InitFromData(positions, normals, texCoords, indices);
    return diamond;
}

Mesh* create_object2D::CreateBackground(const std::string &name, glm::ivec2 resolution) {
    std::vector<glm::vec3> positions = {
        glm::vec3(0, 0, 0),
        glm::vec3(resolution.x, 0, 0),
        glm::vec3(resolution.x, resolution.y, 0),
        glm::vec3(0, resolution.y, 0)
    };

    std::vector<glm::vec3> normals {
        glm::vec3(0, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 0, 1)
    };

    std::vector<glm::vec2> texCoords = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    Mesh* background = new Mesh(name);
    background->InitFromData(positions, normals, texCoords, indices);
    return background;
}