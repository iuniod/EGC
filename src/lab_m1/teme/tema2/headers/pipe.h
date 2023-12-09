#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <string>

#include "utils/glm_utils.h"
#include "lab_m1/teme/tema2/headers/transform3D.h"

class Pipe {
	public:
		Pipe(glm::mat4 modelMatrix, glm::vec3 position, float angle, glm::vec3 dimensions, glm::vec3 center, glm::vec3 peakPosition, float radius);
		~Pipe();

		void Move();
		void Move(float position, float sensitivityOY);

		glm::vec3 position;
		glm::vec3 dimensions;
		glm::vec3 peakPosition;
		glm::vec3 center;
		glm::mat4 modelMatrix;
		float angle;
		float angleDirection;
		float radius;
};
