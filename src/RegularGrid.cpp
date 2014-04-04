#include "RegularGrid.h"


RegularGrid::RegularGrid(std::vector<Object*> objs){
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox* BBox = (*it)->getBoundingBox();
		// Planes overrated -> remove from count
		if (it == objs.begin()){
			_min = BBox->pmin;
			_max = BBox->pmax;
		}
		else {
			if (_min.x > BBox->pmin.x) _min.x = BBox->pmin.x;
			if (_min.y > BBox->pmin.y) _min.y = BBox->pmin.y;
			if (_min.z > BBox->pmin.z) _min.z = BBox->pmin.z;
			if (_max.x < BBox->pmax.x) _max.x = BBox->pmax.x;
			if (_max.y < BBox->pmax.y) _max.y = BBox->pmax.y;
			if (_max.z < BBox->pmax.z) _max.z = BBox->pmax.z;
		}
	}
	
	//Depois disto, os valores mínimos e máximos estão feitos, e logo podemos calcular o tamanho da Grid.
	_NX = _max.x - _min.x;
	_NY = _max.y - _min.y;
	_NZ = _max.z - _min.z;

	//Inicializamos a grid
	_grid.resize(_NX*_NY*_NZ);

	//Adicionamos os objectos às células respectivas
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox* BBox = (*it)->getBoundingBox();

		int ixmin = CLAMP((BBox->pmin.x - _min.x) * _NX / (_max.x - _min.x), 0, _NX - 1);
		int iymin = CLAMP((BBox->pmin.y - _min.y) * _NY / (_max.y - _min.y), 0, _NY - 1);
		int izmin = CLAMP((BBox->pmin.z - _min.z) * _NZ / (_max.z - _min.z), 0, _NZ - 1);
		
		int ixmax = CLAMP((BBox->pmax.x - _min.x) * _NX / (_max.x - _min.x), 0, _NX - 1);
		int iymax = CLAMP((BBox->pmax.y - _min.y) * _NY / (_max.y - _min.y), 0, _NY - 1);
		int izmax = CLAMP((BBox->pmax.z - _min.z) * _NZ / (_max.z - _min.z), 0, _NZ - 1);

		for (int z = izmin; z <= izmax; z++){
			for (int y = iymin; y <= iymax; y++){
				for (int x = ixmin; x <= ixmax; x++){
					int index = x + _NX * y + _NX * _NY * z;
					_grid[index]._objects.push_back(*it);				}			}		}	}
}

