#include "Cylinder.h"


Cylinder::Cylinder(glm::vec3 pa, glm::vec3 pb, float r, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) : Object(fill_color, k_constants, transmittance, indexRefraction, texture){
	//Centro das caps
	_pa = pa;
	_pb = pb;
	//Raio do cilindro
	_r = r;

}

bool Cylinder::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	glm::vec3 v = ray.D;
	glm::vec3 p = ray.O;
	glm::vec3 dp = p - _pa;

	//Direccao do cilindro
	glm::vec3 va = glm::normalize(_pb - _pa);

	float A = glm::dot((v - glm::dot(v, va)* va), (v - glm::dot(v, va)* va));
	float B = 2 * glm::dot(v - glm::dot(v, va)* va, dp - glm::dot(dp, va)* va);
	float C = glm::dot((dp - glm::dot(dp, va)* va), (dp - glm::dot(dp, va)* va)) - (_r * _r);

	float t1 = (-B + sqrt((B*B) - (4 * A *C))) / (2 * A);
	float t2 = (-B - sqrt((B*B) - (4 * A *C))) / (2 * A);

	//O raio nao atinge o cilindro
	if (t1 < 0 && t2 < 0)
		return false;

	//Distancia ao ponto de intercessao
	if (t1 < t2 && t1 > 0)
		_t = t1;
	else if (t2 > 0)
		_t = t2;
	else return false;

	//Ponto de intercessao
	point= glm::vec3(ray.O.x + ray.D.x * _t, ray.O.y + ray.D.y * _t, ray.O.z + ray.D.z * _t);

	//Calculo da normal
	glm::vec3 v1 = point - _pa;
	glm::vec3 v2 = glm::dot(v1, va) * va;
	normal = (v1 - v2);
	return true;

}

glm::vec3 Cylinder::getColorUV(glm::vec3 point, glm::vec3 normal){

	return _fill_color;
}