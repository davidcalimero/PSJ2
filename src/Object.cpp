#include "Object.h"



Object::Object(glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) {
	_fill_color = fill_color;
	_k_constants = k_constants;
	_transmittance = transmittance;
	_indexRefraction = indexRefraction;
}

glm::vec3 Object::GetFillColor(){
	return _fill_color;
}

glm::vec3 Object::Get_k_constants(){
	return _k_constants;
}