#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <string>

#include "utils/glm_utils.h"
#include "lab_m1/teme/tema2/headers/bullet.h"
#include "lab_m1/teme/tema2/headers/turela.h"
#include "lab_m1/teme/tema2/headers/pipe.h"
#include "lab_m1/teme/tema2/headers/building.h"
#include "lab_m1/teme/tema2/headers/transform3D.h"

struct cell {
	int cost;
	int parent;
	bool visited;
};

class Tank {
	public:
		Tank(glm::vec3 position, float angle, glm::vec3 dimensions, float radius, int lives);
		~Tank();

		void Move(float deltaTimeSeconds, std::vector<Tank*>, std::vector<Building*> buildings);
		void Move(float position, float sensitivityOY);
		void MoveFront(float deltaTimeSeconds, int speed, glm::vec4 groundDimensions);
		void MoveBack(float deltaTimeSeconds, int speed, glm::vec4 groundDimensions);
		void RotateLeft(float deltaTimeSeconds, int speed, glm::vec4 groundDimensions);
		void RotateRight(float deltaTimeSeconds, int speed, glm::vec4 groundDimensions);
		void Shoot();

		void SpawnEnemy(glm::vec4 groundCoordinates, std::vector<Building*> buildings, Tank* tank, std::vector<Tank*> enemies);
		void RotateAndShoot(Tank* tank, float deltaTimeSeconds, std::vector<Building*> buildings);
		
		std::vector<glm::vec2> GetVertices2D();
		std::vector<glm::vec2> GetVerticesWithPipe2D();
		
		glm::vec3 dimensions;
		glm::vec3 position;
		glm::vec3 center;
		glm::mat4 modelMatrix;
		float angle;
		float radius;
		bool isAlive;
		int lives;
		Turela* turela;
		Pipe* pipe;
		std::vector<Bullet*> bullets;
	
	private:
		bool isOutOfGround(glm::vec4 groundCoordinates);
		bool arePolygonsOverlapping(std::vector<glm::vec2> polygon1, std::vector<glm::vec2> polygon2);
		bool isInBuildings(std::vector<glm::vec2> tankVertices, std::vector<Building*> buildings);

		float shootCountDown;
		std::vector<std::vector<cell>> matrix;
};
