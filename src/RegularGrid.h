#pragma once

#include "Object.h"

typedef struct {
	std::vector<Object*> objects;
} Cell;

class RegularGrid
{
	private:
		int _NX, _NY, _NZ;
		glm::vec3 _min, _max;
		std::vector<Cell> _grid;
		bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &tMin, glm::vec3 &tMax);
		bool isRayInsideGrid(Ray ray);

	public:
		RegularGrid(std::vector<Object*> objs);
		std::vector<Object*> traversalAlgorithm(Ray ray);
		std::vector<Object*> traversalAlgorithm2(Ray ray);
};

