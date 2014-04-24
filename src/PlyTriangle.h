#pragma once

#include "Triangle.h"

class PlyTriangle : public Triangle {

private:
	glm::vec3 _na;
	glm::vec3 _nb;
	glm::vec3 _nc;

	glm::vec2 _uva;
	glm::vec2 _uvb;
	glm::vec2 _uvc;

	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);

public:
	PlyTriangle(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	virtual glm::vec3 getColorUV(glm::vec3 point, glm::vec3 normal);
};