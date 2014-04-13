#include "PlyTriangle.h"

PlyTriangle::PlyTriangle(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) : Triangle(vertices, fill_color, k_constants, transmittance, indexRefraction, texture){

	_na = normals[0];
	_nb = normals[1];
	_nc = normals[2];

	_uva = uvs[0];
	_uvb = uvs[1];
	_uvc = uvs[2];
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

glm::vec3 PlyTriangle::getColorUV(glm::vec3 point, glm::vec3 normal){

	float A1 = glm::dot(_normal, glm::cross(_c - _b, point - _b)) / 2;
	float A2 = glm::dot(_normal, glm::cross(_a - _c, point - _c)) / 2;
	float A3 = glm::dot(_normal, glm::cross(_b - _a, point - _a)) / 2;

	float AT = A1 + A2 + A3;

	float L1 = A1 / AT;
	float L2 = A2 / AT;
	float L3 = A3 / AT;

	glm::vec2 uv = L1 * _uva + L2 * _uvb + L3 * _uvc;

	//PRINT(uv.x << " " << uv.y);
	int u = abs(uv.x * _texture->size_x);
	int v = abs(uv.y * _texture->size_y);

	glm::vec3 color;
	color.r = _texture->data[(v + u*_texture->size_y)*3 + 0 ] / 255.0f;
	color.g = _texture->data[(v + u*_texture->size_y)*3 + 1 ] / 255.0f;
	color.b = _texture->data[(v + u*_texture->size_y)*3 + 2 ] / 255.0f;
	 
	//PRINT(color.r);

	return color;
}