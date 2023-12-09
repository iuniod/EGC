#include "lab_m1/teme/tema2/headers/turela.h"

using namespace std;
using namespace glm;

Turela::Turela(mat4 modelMatrix, vec3 position, float angle, vec3 dimensions, vec3 center, float radius) {
	this->modelMatrix = modelMatrix;
	this->position = position;
	this->angle = angle;
	this->dimensions = dimensions;
	this->center = center;
	this->radius = radius;
}

Turela::~Turela() {
}

void Turela::Move() {
	this->modelMatrix = translate(mat4(1), this->position);
	this->modelMatrix *= transform3D::RotateOY(this->angle);
}

void Turela::Move(float position, float sensitivityOY) {
	this->modelMatrix = translate(this->modelMatrix, this->center);
	this->modelMatrix *= transform3D::RotateOY(position * sensitivityOY);
	this->modelMatrix = translate(this->modelMatrix, -this->center);
}
