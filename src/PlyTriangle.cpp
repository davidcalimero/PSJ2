#include "PlyTriangle.h"

PlyTriangle::PlyTriangle(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Triangle(vertices, fill_color, k_constants, transmittance, indexRefraction){

	_na = normals[0];
	_nb = normals[1];
	_nc = normals[2];

}

bool PlyTriangle::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	float B, Y, ti;

	// Triangle defined by:
	// p(B,Y) = a + B(b-a) + Y(c-a)
	calculateTriangle(ray, B, Y, ti);

	if (ti < 0) return false;

	if (B < 0 || B > 1 || Y < 0 || Y > 1 || (B + Y) > 1)
		return false;

	_t = ti;

	normal = glm::normalize((1 - B - Y) * _na + B * _nb + Y * _nc);
	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);

	return true;
}