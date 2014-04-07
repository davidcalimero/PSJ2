#include "RegularGrid.h"


RegularGrid::RegularGrid(std::vector<Object*> objs){
	
	int objsGuarded = 0;
	
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox BBox = (*it)->getBoundingBox();
		objsGuarded++;
		// Planes overrated -> remove from count
		if (it == objs.begin()){
			_min = BBox.pmin;
			_max = BBox.pmax;
		}
		else {
			if (_min.x > BBox.pmin.x) _min.x = BBox.pmin.x;
			if (_min.y > BBox.pmin.y) _min.y = BBox.pmin.y;
			if (_min.z > BBox.pmin.z) _min.z = BBox.pmin.z;
			if (_max.x < BBox.pmax.x) _max.x = BBox.pmax.x;
			if (_max.y < BBox.pmax.y) _max.y = BBox.pmax.y;
			if (_max.z < BBox.pmax.z) _max.z = BBox.pmax.z;
		}
	}
	
	//Depois disto, os valores minimos e maximos estao feitos, e logo podemos calcular o tamanho da Grid.
	_NX = (int) ceil((_max.x - _min.x) / CELL_SIZE);
	_NY = (int) ceil((_max.y - _min.y) / CELL_SIZE);
	_NZ = (int) ceil((_max.z - _min.z) / CELL_SIZE);

	//Inicializamos a grid
	_grid.resize(_NX*_NY*_NZ);

	//Adicionamos os objectos as celulas respectivas
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox BBox = (*it)->getBoundingBox();

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

	//DEBUG - Verify if planes are removed from everything - TODO
	std::cout << "Objects in total: " << objs.size() << std::endl;
	std::cout << "Objects without planes involved: " << objsGuarded << std::endl;
}

