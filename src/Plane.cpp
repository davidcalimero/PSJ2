#include "Plane.h"



Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction) {
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;

	_normal = glm::cross(_v3 - _v2, _v2 - _v1);
}

bool Plane::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	normal = _normal;
	float D = (_normal.x * ray.O.x + _normal.y * ray.O.y + _normal.z * ray.O.z) / _normal.length() * 1.0f;

	float denominator = glm::dot(_normal, ray.D);
	if (denominator == 0) return false;
	if (denominator > 0) return false;

	float ti = -(glm::dot(_normal, ray.O) + D) / denominator*1.0f;

	if (ti < 0) return false;

	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);

	return true;
}