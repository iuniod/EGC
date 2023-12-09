#include "lab_m1/teme/tema2/headers/object3D.h"

using namespace std;
using namespace glm;

Mesh* create_object3D::CreateGround(const string &name, vec2 resolution) {
    vector<vec3> positions = {
        vec3(0, 0, 0),
        vec3(resolution.x, 0, 0),
        vec3(resolution.x, 0, resolution.y),
        vec3(0, 0, resolution.y)
    };

    vector<vec3> normals {
        vec3(0, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 1, 0),
        vec3(0, 1, 0)
    };

    vector<vec2> texCoords = {
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(0, 1)
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    Mesh* background = new Mesh(name);
    background->InitFromData(positions, normals, texCoords, indices);
    return background;
}

Mesh* create_object3D::CreateBuilding(const string &name, vec3 center, float width, float height, float depth) {
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float halfDepth = depth / 2;

    vector<vec3> positions = {
        vec3(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z - halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y - halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z - halfDepth),
        vec3(center.x - halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth),
        vec3(center.x + halfWidth, center.y + halfHeight, center.z - halfDepth)
    };

    vector<vec3> normals {
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0)
    };

    vector<vec2> texCoords = {
        vec2(0, 0),
        vec2(width, 0),
        vec2(width, height),
        vec2(0, height),
        vec2(0, 0),
        vec2(width, 0),
        vec2(width, height),
        vec2(0, height),
        vec2(0, 0),
        vec2(depth, 0),
        vec2(depth, height),
        vec2(0, height),
        vec2(0, 0),
        vec2(depth, 0),
        vec2(depth, height),
        vec2(0, height),
        vec2(0, 0),
        vec2(width, 0),
        vec2(width, depth),
        vec2(0, depth),
        vec2(0, 0),
        vec2(width, 0),
        vec2(width, depth),
        vec2(0, depth)
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        8, 9, 10,
        8, 10, 11,
        12, 14, 13,
        12, 15, 14,
        16, 17, 18,
        16, 18, 19,
        20, 22, 21,
        20, 23, 22
    };

    Mesh* building = new Mesh(name);
    building->InitFromData(positions, normals, texCoords, indices);
    return building;
}

Mesh* create_object3D::CreateParalelipiped(const string &name, vec3 lowestCorner, vec3 highestCorner) {
    vector<vec3> positions = {
        vec3(lowestCorner.x, lowestCorner.y, lowestCorner.z),
        vec3(highestCorner.x, lowestCorner.y, lowestCorner.z),
        vec3(highestCorner.x, highestCorner.y, lowestCorner.z),
        vec3(lowestCorner.x, highestCorner.y, lowestCorner.z),
        vec3(lowestCorner.x, lowestCorner.y, highestCorner.z),
        vec3(highestCorner.x, lowestCorner.y, highestCorner.z),
        vec3(highestCorner.x, highestCorner.y, highestCorner.z),
        vec3(lowestCorner.x, highestCorner.y, highestCorner.z)
    };

    vector<vec3> normals {
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, -1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(-1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(1, 0, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, -1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 0)
    };

    vector<vec2> texCoords = {
        vec2(0, 0),
        vec2(highestCorner.x - lowestCorner.x, 0),
        vec2(highestCorner.x - lowestCorner.x, highestCorner.y - lowestCorner.y),
        vec2(0, highestCorner.y - lowestCorner.y),
        vec2(0, 0),
        vec2(highestCorner.z - lowestCorner.z, 0),
        vec2(highestCorner.z - lowestCorner.z, highestCorner.y - lowestCorner.y),
        vec2(0, highestCorner.y - lowestCorner.y),
        vec2(0, 0),
        vec2(highestCorner.x - lowestCorner.x, 0),
        vec2(highestCorner.x - lowestCorner.x, highestCorner.z - lowestCorner.z),
        vec2(0, highestCorner.z - lowestCorner.z),
        vec2(0, 0),
        vec2(highestCorner.z - lowestCorner.z, 0),
        vec2(highestCorner.z - lowestCorner.z, highestCorner.y - lowestCorner.y),
        vec2(0, highestCorner.y - lowestCorner.y),
        vec2(0, 0),
        vec2(highestCorner.x - lowestCorner.x, 0),
        vec2(highestCorner.x - lowestCorner.x, highestCorner.z - lowestCorner.z),
        vec2(0, highestCorner.z - lowestCorner.z),
        vec2(0, 0),
        vec2(highestCorner.x - lowestCorner.x, 0),
        vec2(highestCorner.x - lowestCorner.x, highestCorner.z - lowestCorner.z),
        vec2(0, highestCorner.z - lowestCorner.z)
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        8, 9, 10,
        8, 10, 11,
        12, 14, 13,
        12, 15, 14,
        16, 17, 18,
        16, 18, 19,
        20, 22, 21,
        20, 23, 22
    };

    Mesh* paralelipiped = new Mesh(name);
    paralelipiped->InitFromData(positions, normals, texCoords, indices);
    return paralelipiped;
}
