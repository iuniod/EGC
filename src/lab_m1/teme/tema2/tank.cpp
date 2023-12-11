#include "lab_m1/teme/tema2/headers/tank.h"

using namespace std;
using namespace glm;

// Internal functions used
vec2 rotate(vec2 point, float angle, vec2 center);

Tank::Tank(vec3 position, float angle, vec3 dimensions, float radius, int lives) {
	this->position = position;
	this->angle = angle;
	this->dimensions = dimensions;
	this->center = vec3(0.0f, dimensions.y / 2.0f, dimensions.z / 2.0f - 1.291744f);
	this->radius = radius;
	this->modelMatrix = translate(mat4(1), position);
	this->isAlive = true;
	this->lives = lives;
	this->shootCountDown = 1.0f;

	// Turela
	vec3 dim = vec3(0.751644f, 0.257038f, 0.694881f);
	vec3 center = vec3(0.0f, 0.932061f, -0.389490f);
	float rad = 1.0236348f;
	this->turela = new Turela(this->modelMatrix, this->position, this->angle, dim, center, rad);

	// Pipe
	dim = vec3(0.198852f, 0.209084f, 2.41273f);
	center = vec3(0.0f, 0.913007f, -0.35973f);
	vec3 peakPosition = vec3(0.0f, 0.913007f, 2.053f);
	rad = 2.41273f;
	this->pipe = new Pipe(this->modelMatrix, this->position, this->angle, dim, center, peakPosition, rad);

	// Bullets
	this->bullets = vector<Bullet*>();
}

Tank::~Tank() {
	delete this->turela;
	delete this->pipe;
	for (int i = 0; i < this->bullets.size(); i++) {
		delete this->bullets[i];
	}
	this->bullets.clear();
}

void Tank::Move(float deltaTimeSeconds, vector<Tank*> enemies, vector<Building*> buildings) {
	// check colision with other tanks
	vec3 tankPosition = vec3(this->position.x, 0.0f, this->position.z);
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isAlive) {
			vec3 enemyPosition = vec3(enemies[i]->position.x, 0.0f, enemies[i]->position.z);
			float dist = distance(tankPosition, enemyPosition);
			if (dist < this->radius * 2.0f) {
				float P = abs(this->radius * 2.0f - dist);
				vec3 diff = tankPosition - enemyPosition;
				vec3 correction = normalize(diff) * P;
				this->position += correction * 0.5f;
				this->turela->position += correction * 0.5f;
				this->pipe->position += correction * 0.5f;

				enemies[i]->position -= correction * 0.5f;
				enemies[i]->turela->position -= correction * 0.5f;
				enemies[i]->pipe->position -= correction * 0.5f;
			}
		}
	}

	// check colision with buildings
	for (int i = 0; i < buildings.size(); i++) {
		vec3 buildingPosition = buildings[i]->position;
		float dist = distance(tankPosition, buildingPosition);
		if (dist < this->radius + buildings[i]->radius) {
			float P = abs(this->radius + buildings[i]->radius - dist);
			vec3 diff = tankPosition - buildingPosition;
			vec3 correction = normalize(diff) * P;
			this->position += correction * 0.5f;
			this->turela->position += correction * 0.5f;
			this->pipe->position += correction * 0.5f;
		}
	}

	this->modelMatrix = translate(mat4(1), this->position);
	this->modelMatrix *= transform3D::RotateOY(this->angle);

	this->turela->Move();
	this->pipe->Move();

	this->shootCountDown -= deltaTimeSeconds;
}

void Tank::Move(float position, float sensitivityOY) {
	this->turela->Move(position, sensitivityOY);
	this->pipe->Move(position, sensitivityOY);
}

bool Tank::isOutOfGround(vec4 groundCoordinates) {
	vec2 groundBottomRight = vec2(groundCoordinates.z, groundCoordinates.x);
	vec2 groundTopLeft = vec2(groundCoordinates.y, groundCoordinates.w);

	vec2 distance;
	distance.x = abs(this->position.x - (groundTopLeft.x + groundBottomRight.x) / 2.0f);
	distance.y = abs(this->position.z - (groundTopLeft.y + groundBottomRight.y) / 2.0f);

	vec2 half_ground = vec2((groundBottomRight.x - groundTopLeft.x) / 2.0f, (groundTopLeft.y - groundBottomRight.y) / 2.0f);

	if (distance.x + this->radius <= half_ground.x && distance.y + this->radius <= half_ground.y) {
		return false;
	}

	return true;
}

void Tank::MoveFront(float deltaTime, int speed, vec4 groundDimensions) {
	// Simulate movement
	vec3 forward = vec3(sin(this->angle), 0, cos(this->angle));
	this->position += deltaTime * speed * forward;

	if (isOutOfGround(groundDimensions)) {
		this->position -= deltaTime * speed * forward;
		return;
	}

	// Move turela and pipe
	this->turela->position += deltaTime * speed * forward;
	this->pipe->position += deltaTime * speed * forward;
}

void Tank::MoveBack(float deltaTime, int speed, vec4 groundDimensions) {
	// Simulate movement
	vec3 forward = vec3(sin(this->angle), 0, cos(this->angle));
	this->position -= deltaTime * speed * forward;

	if (isOutOfGround(groundDimensions)) {
			this->position += deltaTime * speed * forward;
		return;
	}

	// Move turela and pipe
	this->turela->position -= deltaTime * speed * forward;
	this->pipe->position -= deltaTime * speed * forward;
}

void Tank::RotateLeft(float deltaTime, int speed, vec4 groundDimensions) {
	// Simulate movement
	this->angle += speed * deltaTime;

	// Rotate turela and pipe
	this->turela->angle += speed * deltaTime;
	this->pipe->angle += speed * deltaTime;
}

void Tank::RotateRight(float deltaTime, int speed, vec4 groundDimensions) {
	// Simulate movement
	this->angle -= speed * deltaTime;
	
	// Rotate turela and pipe
	this->turela->angle -= speed * deltaTime;
	this->pipe->angle -= speed * deltaTime;
}

void Tank::SpawnEnemy(vec4 groundCoordinates, vector<Building*> buildings, Tank* tank, vector<Tank*> enemies) {
	bool overlap = true;
	float x, z;
	while (overlap) {
		x = (float) (rand() % 50 - 25);
		z = (float) (rand() % 50 - 25);

		// Check if the enemy is in the tank
		vec2 enemyPosition = vec2(x, z);
		vec2 tankPosition = vec2(tank->position.x, tank->position.z);
		if (distance(enemyPosition, tankPosition) < 10.0f) {
			continue;
		}

		// Check if the enemy is in another enemy
		for (int i = 0; i < enemies.size(); i++) {
			vec2 enemy2Position = vec2(enemies[i]->position.x, enemies[i]->position.z);
			if (distance(enemyPosition, enemy2Position) < tank->radius * 3.0f) {
				continue;
			}
		}

		// Check if enemy is in a building
		if (isInBuildings(this->GetVerticesWithPipe2D(), buildings)) {
			continue;
		}

		// Check if enemy is out of ground
		this->position = vec3(x, this->position.y, z);
		if (isOutOfGround(groundCoordinates)) {
			continue;
		}
		overlap = false;
	}

	// Spawn enemy
	this->turela->position = this->position;
	this->pipe->position = this->position;

	this->modelMatrix = translate(mat4(1), this->position);
	this->turela->modelMatrix = translate(mat4(1), this->turela->position);
	this->pipe->modelMatrix = translate(mat4(1), this->pipe->position);

	this->shootCountDown = 1.0f;
}

void Tank::RotateAndShoot(Tank* tank, float deltaTimeSeconds, vector<Building*> buildings) {
	vec2 enemyPosition = vec2(this->position.x, this->position.z);
	vec2 tankPosition = vec2(tank->position.x, tank->position.z);
	if (distance(enemyPosition, tankPosition) < 10.0f) {
		// Move
		vec3 direction = normalize(tank->position - this->position);
		float angle = atan2(direction.x, direction.z);
		this->angle = angle;
		this->turela->angle = angle;
		this->pipe->angle = angle;

		Move(deltaTimeSeconds, vector<Tank*>(), buildings);

		// Shoot
		this->Shoot();
	}
}

void Tank::Shoot() {
	if (this->shootCountDown <= 0.0f) {
		this->shootCountDown = 1.0f;

		// Spawn bullet
		float angle = this->pipe->angle + this->pipe->angleDirection;

		float x = this->position.x + sin(angle) * (this->pipe->radius + 0.3f);
		float z = this->position.z + cos(angle) * (this->pipe->radius + 0.3f);
		vec3 positionBullet = vec3(x, this->pipe->peakPosition.y, z);

		vec3 offset = vec3(sin(this->pipe->angle), 0.0f, cos(this->pipe->angle)) * 0.459730f;
		Bullet* bullet = new Bullet(positionBullet, angle, 25, 0.4f, -offset);
		this->bullets.push_back(bullet);
	}
}

vector<vec2> Tank::GetVertices2D() {
	// Calculate Center
	vec2 Center = vec2(this->position.x, this->position.z);

	// Calculate Vertices
	vector<vec2> Vertices = vector<vec2>();
	Vertices.push_back(Center + vec2(-this->dimensions.x / 2.0f, -this->dimensions.z / 2.0f));
	Vertices.push_back(Center + vec2(this->dimensions.x / 2.0f, -this->dimensions.z / 2.0f));
	Vertices.push_back(Center + vec2(this->dimensions.x / 2.0f, this->dimensions.z / 2.0f));
	Vertices.push_back(Center + vec2(-this->dimensions.x / 2.0f, this->dimensions.z / 2.0f));

	// Rotate Vertices
	for (int i = 0; i < Vertices.size(); i++) {
		Vertices[i] = rotate(Vertices[i], this->angle, Center);
	}

	return Vertices;
}

vector<vec2> Tank::GetVerticesWithPipe2D() {
	// Calculate Center
	vec2 Center = vec2(this->position.x, this->position.z);

	// Calculate Vertices
	vector<vec2> Vertices = vector<vec2>();
	Vertices.push_back(Center + vec2(-this->dimensions.x / 2.0f, -this->dimensions.z / 2.0f));
	Vertices.push_back(Center + vec2(this->dimensions.x / 2.0f, -this->dimensions.z / 2.0f));
	Vertices.push_back(Center + vec2(this->dimensions.x / 2.0f, this->pipe->peakPosition.z));
	Vertices.push_back(Center + vec2(-this->dimensions.x / 2.0f, this->pipe->peakPosition.z));

	// Rotate Vertices
	for (int i = 0; i < Vertices.size(); i++) {
		Vertices[i] = rotate(Vertices[i], -this->angle, Center);
	}

	return Vertices;
}

bool Tank::arePolygonsOverlapping(vector<vec2> polygon1, vector<vec2> polygon2) {
	for (auto polygon : {polygon1, polygon2}) {
		for (int i = 0; i < polygon.size(); i++) {
			int j = (i + 1) % polygon.size();
			vec2 edge = polygon[j] - polygon[i];
			vec2 normal = vec2(edge.y, -edge.x);

			float minA = numeric_limits<float>::max();
			float maxA = numeric_limits<float>::min();
			for (int k = 0; k < polygon1.size(); k++) {
				float projected = normal.x * polygon1[k].x + normal.y * polygon1[k].y;
				if (projected < minA) {
					minA = projected;
				}
				if (projected > maxA) {
					maxA = projected;
				}
			}

			float minB = numeric_limits<float>::max();
			float maxB = numeric_limits<float>::min();
			for (int k = 0; k < polygon2.size(); k++) {
				float projected = normal.x * polygon2[k].x + normal.y * polygon2[k].y;
				if (projected < minB) {
					minB = projected;
				}
				if (projected > maxB) {
					maxB = projected;
				}
			}

			if (maxA < minB || maxB < minA) {
				return false;
			}
		}
	}
	return true;
}

bool Tank::isInBuildings(vector<vec2> tankVertices, vector<Building*> buildings) {
    // Check if tank is in a building
	for (int i = 0; i < buildings.size(); i++) {
		vector<vec2> buildingVertices = buildings[i]->GetVertices2D();
		if (arePolygonsOverlapping(tankVertices, buildingVertices)) {
			return true;
		}
	}

	return false;
}

vec2 rotate(vec2 point, float angle, vec2 center) {
	point.x -= center.x;
	point.y -= center.y;

	float x = point.x * cos(angle) - point.y * sin(angle);
	float y = point.x * sin(angle) + point.y * cos(angle);

	point.x = x + center.x;
	point.y = y + center.y;

	return point;
}
