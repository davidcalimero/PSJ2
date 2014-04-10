#pragma once

#include "Object.h"

typedef struct {
	std::vector<Object*> _objects;
} Cell;

class RegularGrid
{
	private:
		int _NX, _NY, _NZ;
		glm::vec3 _min, _max;
		std::vector<Cell> _grid;
		bool rayInterception(Ray ray);

	public:
		RegularGrid(std::vector<Object*> objs);
		std::vector<Object*> traversalAlgorithm(Ray ray);
};

