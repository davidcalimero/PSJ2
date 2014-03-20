#pragma once

#include <iostream>
#include <glm.hpp>
#include "Utils.h"


class Object {


protected:
	glm::vec3 _fill_color;
	glm::vec3 _k_constants;
	float _transmittance;
	float _indexRefraction;

public:
	Object(glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);
	virtual bool rayInterception(Ray ray, glm::vec3 &point) = 0;

	glm::vec3 GetFillColor();

};