#pragma once

#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"


class Scene {
private:
	Camera* _camera;
	glm::vec3 _bckg_color;

	std::vector<Light*> _lights;
	std::vector<Object*> _objects;

	bool loadNFF(char* filename, glm::vec3 &from, glm::vec3 &at, glm::vec3 &up, float &angle, float &hither, glm::vec2 &resolution, glm::vec3 &bckg_color, std::vector<Light*> &lights, std::vector<Object*> &objects);


public:
	Scene();
	~Scene();
	Camera* GetCamera();
	glm::vec3 GetBckgColor();
	bool loadNFF(char* filename);

	std::vector<Object*> GetObjects();
};