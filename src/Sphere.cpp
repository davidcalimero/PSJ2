#include "Sphere.h"



Sphere::Sphere(glm::vec3 centro, float raio, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction){
	_centro = centro;
	_raio = raio;
}