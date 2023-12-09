#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <string>

#include "utils/glm_utils.h"
#include "lab_m1/teme/tema2/headers/transform3D.h"

class Turela {
	public:
		Turela(glm::mat4 modelMatrix, glm::vec3 position, float angle, glm::vec3 dimensions, glm::vec3 center, float radius);
		~Turela();

		void Move();
		void Move(float position, float sensitivityOY);

		glm::vec3 position;
		glm::vec3 dimensions;
		glm::vec3 center;
		glm::mat4 modelMatrix;
		float angle;
		float radius;
};
