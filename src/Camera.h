#pragma once

#include "Utils.h"


class Camera {

private:
	glm::vec3 _eye, _at, _up;
	float _fovy;
	float _near, _far; //hither and yon planes 
	int _ResX, _ResY;

	float _d;
	float _w, _h;
	glm::vec3 _xe, _ye, _ze; //uvn frame _xe -> u, _ye -> v, _ze -> n

public:
	Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float fovy, float n, float f, int ResX, int ResY);
	~Camera();
	int GetResX();
	int GetResY();
	Ray PrimaryRay(float x, float y);
	glm::vec3 GetPos();
	glm::vec3 GetU();
	glm::vec3 GetV();

};