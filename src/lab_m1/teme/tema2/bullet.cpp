#include "lab_m1/teme/tema2/headers/bullet.h"

using namespace std;
using namespace glm;

Bullet::Bullet(vec3 position, float angle, float speed, float timeToLive, vec3 offset) {
	this->position = position;
	this->angle = angle;
	this->speed = speed;
	this->timeToLive = timeToLive;
	this->isAlive = true;

	this->modelMatrix = mat4(1);
	this->modelMatrix = translate(this->modelMatrix, this->position + offset);
	this->modelMatrix = scale(this->modelMatrix, vec3(0.13f));
}

Bullet::~Bullet() {
}

void Bullet::Move(float deltaTime) {
	this->position += vec3(sin(this->angle), 0.0f, cos(this->angle)) * this->speed * deltaTime;
	this->timeToLive -= deltaTime;
	this->modelMatrix = translate(mat4(1), this->position);
	this->modelMatrix = scale(this->modelMatrix, vec3(0.13f));

	if (this->timeToLive <= 0.0f) {
		this->isAlive = false;
		cout << "Bullet died at position: " << this->position.x << " " << this->position.y << " " << this->position.z << endl;
	}
}
