#pragma once

#include "Utils.h"
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

	public:
		RegularGrid(std::vector<Object*> objs);
};

