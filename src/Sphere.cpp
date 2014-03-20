#include "Sphere.h"



Sphere::Sphere(glm::vec3 centro, float raio, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction){
	_centro = centro;
	_raio = raio;
}

bool Sphere::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	float doc2 = (ray.O.x - _centro.x) * (ray.O.x - _centro.x) + (ray.O.y - _centro.y) * (ray.O.y - _centro.y) + (ray.O.z - _centro.z) * (ray.O.z - _centro.z);
	if (doc2 == (_raio * _raio)) return false;

	float B = ray.D.x * (_centro.x - ray.O.x) + ray.D.y * (_centro.y - ray.O.y) + ray.D.z * (_centro.z - ray.O.z);
	if (doc2 > (_raio * _raio) && B < 0) return false;

	float R = B * B - doc2 + _raio * _raio;
	if (R < 0) return false;

	float ti;
	if (doc2 > (_raio * _raio)) ti = B - sqrt(R);
	if (doc2 < (_raio * _raio)) ti = B + sqrt(R);

	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);
	
	normal.x = (point.x - _centro.x) / _raio;
	normal.y = (point.y - _centro.y) / _raio;
	normal.z = (point.z - _centro.z) / _raio;
	
	normal = glm::normalize(normal);
	return true;
}