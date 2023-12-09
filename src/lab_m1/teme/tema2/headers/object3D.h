#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace create_object3D {
    Mesh* CreateGround(const std::string &name, glm::vec2 resolution);

    Mesh* CreateBuilding(const std::string &name, glm::vec3 center, float width, float height, float depth);

    Mesh* CreateParalelipiped(const std::string &name, glm::vec3 lowestCorner, glm::vec3 highestCorner);
}
