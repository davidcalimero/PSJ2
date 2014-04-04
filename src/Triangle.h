#pragma once

#include "Object.h"


class Triangle : public Object {

private:
	glm::vec3 _a;
	glm::vec3 _b;
	glm::vec3 _c;

	glm::vec3 _normal;

public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);
	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);

};