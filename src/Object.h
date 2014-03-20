#pragma once

#include <iostream>
#include <glm.hpp>



class Object {

private:

protected:
	glm::vec3 _fill_color;
	glm::vec3 _k_constants;
	float _transmittance;
	float _indexRefraction;

public:
	Object(glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);

};