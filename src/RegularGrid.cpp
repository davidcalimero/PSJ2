#include "RegularGrid.h"


RegularGrid::RegularGrid(std::vector<Object*> objs){

	int objsGuarded = 0;
	
	if (objs.size() != 0){
		_min = (*objs.begin())->getBoundingBox().pmin;
		_max = (*objs.begin())->getBoundingBox().pmax;
	}

	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox BBox = (*it)->getBoundingBox();

		//DEBUG - Check if there are planes. They are done by having pmin = pmax
		/** /
		std::cout << "pmin: (" << BBox.pmin.x << ", " << BBox.pmin.y << ", "  << BBox.pmin.z << ")" << std::endl;
		std::cout << "pmax: (" << BBox.pmax.x << ", " << BBox.pmax.y << ", " << BBox.pmax.z << ")" << std::endl;
		/**/

		if (BBox.pmin == BBox.pmax)	continue;
		objsGuarded++;

		if (_min.x > BBox.pmin.x) _min.x = BBox.pmin.x;
		if (_min.y > BBox.pmin.y) _min.y = BBox.pmin.y;
		if (_min.z > BBox.pmin.z) _min.z = BBox.pmin.z;
		if (_max.x < BBox.pmax.x) _max.x = BBox.pmax.x;
		if (_max.y < BBox.pmax.y) _max.y = BBox.pmax.y;
		if (_max.z < BBox.pmax.z) _max.z = BBox.pmax.z;
	}
	
	//Depois disto, os valores minimos e maximos estao feitos, e logo podemos calcular o tamanho da Grid.

	float wx = (_max.x - _min.x);
	float wy = (_max.y - _min.y);
	float wz = (_max.z - _min.z);
	float s = pow((wx * wy + wz) / objsGuarded, 1.0f / 3.0f);

	_NX = (int) floor(M * wx / s) + 1;
	_NY = (int) floor(M * wy / s) + 1;
	_NZ = (int) floor(M * wz / s) + 1;

	//Inicializamos a grid
	_grid.resize(_NX*_NY*_NZ);

	//Adicionamos os objectos as celulas respectivas
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox BBox = (*it)->getBoundingBox();
		if (BBox.pmin == BBox.pmax)	continue;

		int ixmin = (int) floor(CLAMP((BBox.pmin.x - _min.x) * _NX / (_max.x - _min.x), 0, _NX - 1));
		int iymin = (int) floor(CLAMP((BBox.pmin.y - _min.y) * _NY / (_max.y - _min.y), 0, _NY - 1));
		int izmin = (int) floor(CLAMP((BBox.pmin.z - _min.z) * _NZ / (_max.z - _min.z), 0, _NZ - 1));
		
		int ixmax = (int) floor(CLAMP((BBox.pmax.x - _min.x) * _NX / (_max.x - _min.x), 0, _NX - 1));
		int iymax = (int) floor(CLAMP((BBox.pmax.y - _min.y) * _NY / (_max.y - _min.y), 0, _NY - 1));
		int izmax = (int) floor(CLAMP((BBox.pmax.z - _min.z) * _NZ / (_max.z - _min.z), 0, _NZ - 1));

		for (int z = izmin; z <= izmax; z++){
			for (int y = iymin; y <= iymax; y++){
				for (int x = ixmin; x <= ixmax; x++){
					int index = x + _NX * y + _NX * _NY * z;
					_grid[index]._objects.push_back(*it);
				}
			}
		}
	}
	//DEBUG - Creation of the cellgrid - DONE
	/** /
	std::cout << "CellGrid: " << std::endl;
	std::cout << "Number of Cells: NX = " << _NX << "; NY = " << _NY << "; NZ = " << _NZ << std::endl;
	std::cout << "min: (" << _min.x << ", " << _min.y << ", " << _min.z << "); " << std::endl << "max: (" << _max.x << ", " << _max.y << ", " << _max.z << ");" << std::endl << std::endl;
	/**/

	//DEBUG - Verify if objects are in their respective cells - DONE
	//NOTE: Each cell goes from 0 to less than their CELL_SIZE value. That means that if CELL_SIZE is 1, it goes from 0 to 0.9999999(9), and not 0 to 1.
	/** /
	int number = 0;
	for (std::vector<Cell>::iterator cell = _grid.begin(); cell != _grid.end(); cell++){
		std::cout << "In Cell " << number << " there is a total of " << cell->_objects.size() << " objects:" << std::endl;
		number++;
		for (std::vector<Object*>::iterator it = cell->_objects.begin(); it != cell->_objects.end(); it++){
			std::cout << typeid(*it).name() << std::endl;
		}
	}
	/**/

	//DEBUG - Verify if planes are removed from everything
	/** /
	std::cout << "Objects in total: " << objs.size() << std::endl;
	std::cout << "Objects without planes involved: " << objsGuarded << std::endl;
	/**/
}

bool RegularGrid::rayInterception(Ray ray){

	float normalDir, tmin, tmax;
	float tprox = -(float)INFINITE;
	float tdist = (float)INFINITE;

	glm::vec3 v0 = ray.O;
	glm::vec3 vd = ray.D;
	glm::vec3 vmin = _min;
	glm::vec3 vmax = _max;

	//Calcular o as intercessoes dos planos x, y e z
	for (int i = 0; i < 3; i++){
		normalDir = 1;

		//Se vd for nulo o raio e paralelo aos planos e se estiver fora do plano naon exister intercessao
		if (vd[i] == 0 && (v0[i] < vmin[i] || v0[i] > vmax[i]))
			return false;

		//tmin e tmax para o plano respectivo
		tmin = (vmin[i] - v0[i]) / vd[i];
		tmax = (vmax[i] - v0[i]) / vd[i];

		if (tmin > tmax){
			SWAP(float, tmin, tmax);
			normalDir = -1;
		}

		if (tmin > tprox){
			tprox = tmin;
		}

		if (tmax < tdist)
			tdist = tmax;

		//Se tprox > tdist o raio nao intersecta e se tdist < 0 o raio aponta na direccao contraria
		if (tprox > tdist || tdist < 0)
			return false;
	}
	return true;
}

std::vector<Object*> RegularGrid::traversalAlgorithm(Ray ray){
	std::vector<Object*> objs;
	//Fazer cálculos! :D
	
	if (!rayInterception(ray))
		return objs;

	// Calcular ponto de interseccao na grid

	// fazer o traverse algorithm

	return objs;
}