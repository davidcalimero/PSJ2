#pragma once

#include "ply.h"
#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Poly.h"
#include "Triangle.h"
#include "PlyTriangle.h"
#include "AABB.h"
#include "RegularGrid.h"
#include "Cylinder.h"

class Scene {
private:
	Camera* _camera;
	glm::vec3 _bckg_color;
	std::vector<Light*> _lights;
	std::vector<Object*> _objects;
	RegularGrid* _grid;
	bool _usingGrid;

	Scene();
	bool loadNFF(char* filename, glm::vec3 &from, glm::vec3 &at, glm::vec3 &up, float &angle, float &hither, glm::vec2 &resolution, glm::vec3 &bckg_color, std::vector<Light*> &lights, std::vector<Object*> &objects);
	void loadPLY(const char *objFile, glm::vec3 pos, std::vector<Object*> &objects, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture);
	// Debug
	void printPropertiesValues(Properties* properties);
	void showFinalValues();



public:
	static Scene& getInstance(); //Singleton
	~Scene();
	Camera* GetCamera();
	glm::vec3 GetBckgColor();
	bool loadNFF(char* filename);

	std::vector<Object*> GetObjects(Ray ray);
	std::vector<Light*> GetLights();
	void toggleGrid();
	bool isUsingGrid();
	
};