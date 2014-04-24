#pragma once

#include "Object.h"


class Triangle : public Object {

protected:
	glm::vec3 _a;
	glm::vec3 _b;
	glm::vec3 _c;

	glm::vec3 _normal;

	void calculateTriangle(Ray ray, float &B, float &Y, float &t);
	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);

public:
	Triangle(std::vector<glm::vec3> vertices, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	virtual glm::vec3 getColorUV(glm::vec3 point, glm::vec3 normal);

};