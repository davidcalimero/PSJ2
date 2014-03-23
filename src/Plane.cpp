#include "Plane.h"



Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction) {
	//Vertices do plano
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;

	//Normal do plano
	_normal = glm::normalize(glm::cross(_v3 - _v2, _v1 - _v2));

	//Mostra se a criacao do plano foi bem feita (DEBUG)
	/** /
	std::cout 	<< "Plane defined by:" << std::endl
	<< " - P1 (" << _v1.x << "," << _v1.y << "," << _v1.z << ") " << std::endl
	<< " - P2 (" << _v2.x << "," << _v2.y << "," << _v2.z << ") " << std::endl
	<< " - P3 (" << _v3.x << "," << _v3.y << "," << _v3.z << ") " << std::endl;
	/**/
}

//Calcula a interseccao do raio com um plano
bool Plane::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	normal = _normal;
	float D = -glm::dot(normal, _v1);

	float denominator = _normal.x * ray.D.x + _normal.y * ray.D.y + _normal.z * ray.D.z;
	//Se o denominador for igual a 0 o plano e o raio sao paralelos e como tal retorna false
	if (denominator == 0) return false;

	float ti = -(_normal.x * ray.O.x + _normal.y * ray.O.y + _normal.z * ray.O.z + D) / denominator;

	//Se a interseccao estiver por tras do raio original retorna false
	if (ti < 0) return false;

	//Ponto onde existe interseccao
	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);

	return true;
}