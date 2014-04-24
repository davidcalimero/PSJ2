#pragma once

#include "Utils.h"


class Object {


protected:
	glm::vec3 _fill_color;
	glm::vec3 _k_constants;
	float _transmittance;
	float _indexRefraction;
	tImageTGA *_texture;
	BoundingBox _box;
	float _t;

	Ray _lastRay;
	glm::vec3 _lastPoint;
	glm::vec3 _lastNormal;

	virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal) = 0;

public:
	unsigned long int _counter;  //Counter of Interceptions

	Object(glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	virtual glm::vec3 getColorUV(glm::vec3 point, glm::vec3 normal) = 0;
	bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal, float &t);
	
	glm::vec3 GetFillColor(glm::vec3 point, glm::vec3 normal);
	glm::vec3 Get_k_constants();
	float getTransmittance();
	float getRefractionIndex();
	BoundingBox getBoundingBox();

	Ray getLastInterception(glm::vec3 &point, glm::vec3 &normal);

};