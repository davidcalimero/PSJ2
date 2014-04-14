#pragma once

#include "Object.h"

class Cylinder : public Object {

private:
	glm::vec3 _pa;
	glm::vec3 _pb;
	float _r;

public:
	Cylinder(glm::vec3 pa, glm::vec3 pb, float r, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);
	glm::vec3 getColorUV(glm::vec3 point, glm::vec3 normal);
};