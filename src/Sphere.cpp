#include "Sphere.h"


Sphere::Sphere(glm::vec3 centro, float raio, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) : Object(fill_color, k_constants, transmittance, indexRefraction, texture){
	_centro = centro;
	_raio = raio;

	_box.pmin = centro - raio;
	_box.pmax = centro + raio;

	// Mostrar se a esfera esta a ser bem criada (DEBUG)
	//std::cout << "sphere: center in (" << _centro.x << "," << _centro.y << "," << _centro.z << ") and with radius " << _raio << std::endl;
}

bool Sphere::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	
	//Quadrado da distancia da origem do raio ao centro da esfera
	float doc2 = (ray.O.x - _centro.x) * (ray.O.x - _centro.x) + (ray.O.y - _centro.y) * (ray.O.y - _centro.y) + (ray.O.z - _centro.z) * (ray.O.z - _centro.z);
	float r2 = _raio * _raio;


	//Se a origem do raio estiver sobre a superficie da esfera retornar false
	if (doc2 == r2) return false;

	float B = ray.D.x * (_centro.x - ray.O.x) + ray.D.y * (_centro.y - ray.O.y) + ray.D.z * (_centro.z - ray.O.z);
	
	//Se o raio apontar para o sentido contrário a localizacao da esfera retornar false
	if (doc2 > r2 && B < 0) return false;

	float R = (B * B) - doc2 + r2;
	if (R < 0) return false;

	float ti;
	if (doc2 >r2) ti = B - sqrt(R);
	if (doc2 < r2 && getTransmittance() == 0.0f) return false;
	if (doc2 < r2) ti = B + sqrt(R);

	//Distancia ao ponto de intercessao
	_t = ti;
	
	//Ponto de interseccao
	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);
	
	//Normal a superficie da esfera no ponto de interseccao
	normal.x = (point.x - _centro.x) / _raio;
	normal.y = (point.y - _centro.y) / _raio;
	normal.z = (point.z - _centro.z) / _raio;

	normal = glm::normalize(normal);

	if (doc2 < r2) normal *= -1;

	return true;
}

glm::vec3 Sphere::getColorUV(glm::vec3 point, glm::vec3 normal){
	glm::vec3 pole(0, 1, 0);
	glm::vec3 equator(1, 0, 0);

	float phi = acos(glm::dot(pole, point));
	float v = phi / (float)PI;
	float u;

	float theta = (acos(glm::dot(normal, equator) / sin(phi))) / (2 * (float)PI);

	if (glm::dot(normal, glm::cross(pole, equator)) > 0)
		u = theta;
	else u = 1 - theta;

	int ut = (int)abs(u * _texture->size_x);
	int vt = (int)abs(v * _texture->size_y);

	glm::vec3 color;
	color.r = _texture->data[(vt + ut*_texture->size_y) * 3 + 0] / 255.0f;
	color.g = _texture->data[(vt + ut*_texture->size_y) * 3 + 1] / 255.0f;
	color.b = _texture->data[(vt + ut*_texture->size_y) * 3 + 2] / 255.0f;
	
	return color;
}