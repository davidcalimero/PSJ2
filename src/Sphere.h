#pragma once

#include "Object.h"


class Sphere : public Object {

private:
	glm::vec3 _centro;
	float _raio;

public:
	Sphere(glm::vec3 centro, float raio, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);
	virtual glm::vec3 getColorUV(glm::vec3 point, glm::vec3 normal);
};