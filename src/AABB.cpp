#include "AABB.h"


AABB::AABB(glm::vec3 pMin, glm::vec3 pMax, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) : Object(fill_color, k_constants, transmittance, indexRefraction,texture){
	_box.pmin = pMin;
	_box.pmax = pMax;
}

bool AABB::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	float normalDir, tmin, tmax;
	float tprox = -(float)INFINITE;
	float tdist = (float)INFINITE;

	glm::vec3 v0 = ray.O;
	glm::vec3 vd = ray.D;
	glm::vec3 vmin = _box.pmin;
	glm::vec3 vmax = _box.pmax;

	//Calcular o as intercessoes dos planos x, y e z
	for (int i = 0; i < 3; i++){
		normalDir = -1;

		//Se vd for nulo o raio e paralelo aos planos e se estiver fora do plano naon exister intercessao
		if (vd[i] == 0 && (v0[i] < vmin[i] || v0[i] > vmax[i]))
			return false;

		//tmin e tmax para o plano respectivo
		tmin = (vmin[i] - v0[i]) / vd[i];
		tmax = (vmax[i] - v0[i]) / vd[i];

		if (tmin > tmax){
			SWAP(float, tmin, tmax);
			normalDir = 1;
		}

		if (tmin > tprox){
			tprox = tmin;
			normal = glm::vec3(0);
			normal[i] = normalDir;
		}

		if (tmax < tdist)
			tdist = tmax;

		//Se tprox > tdist o raio nao intersecta e se tdist < 0 o raio aponta na direccao contraria
		if (tprox > tdist || tdist < 0)
			return false;
	}

	//Distancia ao ponto de intercessao
	_t = tprox;

	//Ponto de intercessao
	point = glm::vec3(ray.O.x + ray.D.x * tprox, ray.O.y + ray.D.y * tprox, ray.O.z + ray.D.z * tprox);

	return true;
}

glm::vec3 AABB::getColorUV(glm::vec3 point, glm::vec3 normal){
	
	// Encontrar os eixos dos vectores u e v
	glm::vec3 uAxis = glm::vec3(normal.y, normal.z, -normal.x);
	glm::vec3 vAxis = glm::cross(uAxis, normal);

	float u = glm::dot(point, uAxis);
	float v = glm::dot(point, vAxis);

	//PRINT(u << " " << v);

	glm::vec3 textureColor;
	int ut = abs(u * _texture->size_x);
	int vt = abs(v * _texture->size_y);

	//PRINT((vt + ut*_texture->size_y) * 3 + 0);
	textureColor.r = _texture->data[(ut + vt*_texture->size_x) * 3 + 0];
	textureColor.g = _texture->data[(ut + vt*_texture->size_x) * 3 + 1];
	textureColor.b = _texture->data[(ut + vt*_texture->size_x) * 3 + 2];

	return textureColor;
}