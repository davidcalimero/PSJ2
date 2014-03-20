#include "Triangle.h"



Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction) {
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;
}