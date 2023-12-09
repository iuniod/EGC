#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace create_object2D
{
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, float length, bool fill = false);

    // Create bar with given bottom left and upper right corners
    Mesh* CreateRedBar(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 rightUpperCorner, bool fill = false);

    // Create polygon with given center, radius, number of sides, color and if it contains another small polygon inside
    Mesh* CreatePentagon(const std::string &name, glm::vec3 center, float radius, int no_sides, glm::vec3 color, bool fill = false, bool doubleFill = false);

    // Create circle with given center, radius, color and if it contains another small circle inside
    Mesh* CreateCircle(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false, bool doubleFill = false);

    // Create hexagon with given center, radius, color and if it contains another small hexagon inside
    Mesh* CreateHexagon(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false, bool doubleFill = false);

    // Create a heart with given center and radius, and return the radius of the heart
    Mesh* CreateHeart(const std::string &name, glm::vec3 center, float radius, float &return_radius, bool fill = false);

    // Create a rocket with given center, radius and color
    Mesh* CreateRocket(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);

    // Create a star with given center, radius and color, and return the radius of the star
    Mesh* CreateStar(const std::string &name, glm::vec3 center, float radius, float &return_radius, glm::vec3 color, bool fill = false);

    // Create a diamond ring with given center, radius and color
    Mesh* CreateDiamondRing(const std::string &name, glm::vec3 center, float radius);

    Mesh* CreateBackground(const std::string &name, glm::ivec2 resolution);
}
