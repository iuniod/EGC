#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <string>

#include "lab_m1/teme/tema2/headers/transform3D.h"
#include "utils/glm_utils.h"

class Bullet {
	public:
		Bullet(glm::vec3 position, float angle, float speed, float timeToLive, glm::vec3 offset);
		~Bullet();

		void Move(float deltaTime);

		glm::vec3 position;
		glm::mat4 modelMatrix;
		float angle;
		float speed;
		float timeToLive;
		bool isAlive;
};
