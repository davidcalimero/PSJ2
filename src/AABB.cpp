#include "AABB.h"


AABB::AABB(glm::vec3 p1, glm::vec3 p2, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction){
	_p1 = p1;
	_p2 = p2;
}

bool AABB::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	
	
	return false;
}
