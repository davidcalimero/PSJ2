#include "Object.h"



Object::Object(glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) {
	_fill_color = fill_color;
	_k_constants = k_constants;
	_transmittance = transmittance;
	_indexRefraction = indexRefraction;
	_texture = texture;
	_t = (float)INFINITE;
}

//Indica o valor de t (distancia ao ponto de intercessao)
bool Object::rayInterception(Ray ray, float &t){
	glm::vec3 point;
	glm::vec3 normal;
	bool intercessao = rayInterception(ray, point, normal);
	t = _t;
	return intercessao;
}

//Retorna a cor do material do objecto
glm::vec3 Object::GetFillColor(glm::vec3 point, glm::vec3 normal){
	
	if (_texture == NULL){
		return _fill_color;
	}
	
	return getColorUV(point, normal) * _fill_color;
}

//Retorna as caracteristicas do material (diffuse, specular, shininess)
glm::vec3 Object::Get_k_constants(){
	return _k_constants;
}

float Object::getTransmittance(){
	return _transmittance;
}

float Object::getRefractionIndex(){
	return _indexRefraction;
}

BoundingBox Object::getBoundingBox(){
	return _box;
}