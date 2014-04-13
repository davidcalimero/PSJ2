#pragma once

#include "Triangle.h"

class PlyTriangle : public Triangle {

private:
	glm::vec3 _na;
	glm::vec3 _nb;
	glm::vec3 _nc;

public:
	PlyTriangle(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);
	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);
};