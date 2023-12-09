#include "lab_m1/teme/tema2/headers/pipe.h"

using namespace std;
using namespace glm;

Pipe::Pipe(mat4 modelMatrix, vec3 position, float angle, vec3 dimensions, vec3 center, vec3 peakPosition, float radius) {
	this->modelMatrix = modelMatrix;
	this->position = position;
	this->angle = angle;
	this->angleDirection = 0.0f;
	this->dimensions = dimensions;
	this->center = center;
	this->peakPosition = peakPosition;
	this->radius = radius;
}

Pipe::~Pipe() {
}

void Pipe::Move(float position, float sensitivityOY) {
	this->modelMatrix = translate(this->modelMatrix, this->center);
	this->modelMatrix *= transform3D::RotateOY(position * sensitivityOY);
	this->modelMatrix = translate(this->modelMatrix, -this->center);

	this->angleDirection = position * sensitivityOY;
}

void Pipe::Move() {
	this->modelMatrix = translate(mat4(1), this->position);
	this->modelMatrix *= transform3D::RotateOY(this->angle);
}