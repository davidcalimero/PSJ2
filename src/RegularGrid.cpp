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

	//Incrementar ou decrementar epsilon, para garantir colisão
	_min -= 0.001;
	_max += 0.001;

	//Depois disto, os valores minimos e maximos estao feitos, e logo podemos calcular o tamanho da Grid.
	float wx = (_max.x - _min.x);
	float wy = (_max.y - _min.y);
	float wz = (_max.z - _min.z);
	float s = (float)pow(wx * wy * wz / (float)objsGuarded, 1.0f / 3.0f);

	_N[0] = (int)trunc(M * wx / s) + 1;
	_N[1] = (int)trunc(M * wy / s) + 1;
	_N[2] = (int)trunc(M * wz / s) + 1;

	//Inicializamos a grid
	_grid.resize(_N[0] * _N[1] * _N[2]);

	//Adicionamos os objectos as celulas respectivas
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		BoundingBox BBox = (*it)->getBoundingBox();
		if (BBox.pmin == BBox.pmax)	continue;

		int ixmin = (int)CLAMP((BBox.pmin.x - _min.x) * _N[0] / (_max.x - _min.x), 0.0f, _N[0] - 1);
		int iymin = (int)CLAMP((BBox.pmin.y - _min.y) * _N[1] / (_max.y - _min.y), 0.0f, _N[1] - 1);
		int izmin = (int)CLAMP((BBox.pmin.z - _min.z) * _N[2] / (_max.z - _min.z), 0.0f, _N[2] - 1);

		int ixmax = (int)CLAMP((BBox.pmax.x - _min.x) * _N[0] / (_max.x - _min.x), 0.0f, _N[0] - 1);
		int iymax = (int)CLAMP((BBox.pmax.y - _min.y) * _N[1] / (_max.y - _min.y), 0.0f, _N[1] - 1);
		int izmax = (int)CLAMP((BBox.pmax.z - _min.z) * _N[2] / (_max.z - _min.z), 0.0f, _N[2] - 1);

		for (int z = izmin; z <= izmax; z++){
			for (int y = iymin; y <= iymax; y++){
				for (int x = ixmin; x <= ixmax; x++){
					int index = x + _N[0] * y + _N[0] * _N[1] * z;
					_grid[index].objects.push_back(*it);
				}
			}
		}
	}
	//DEBUG - Creation of the cellgrid - DONE
	/** /
	std::cout << "CellGrid: " << objsGuarded << std::endl;
	std::cout << "Number of Cells: NX = " << _NX << "; NY = " << _NY << "; NZ = " << _NZ << std::endl;
	std::cout << "wx = " << wx << "; wy = " << wy << "; wz = " << wz << "; s = " << s << std::endl;
	std::cout << "Total of Cells: " << (_NX * _NY * _NZ) << " -->" << (M * M * M * objsGuarded) << std::endl;
	std::cout << "min: (" << _min.x << ", " << _min.y << ", " << _min.z << "); " << std::endl << "max: (" << _max.x << ", " << _max.y << ", " << _max.z << ");" << std::endl << std::endl;
	/**/

	//DEBUG - Verify if objects are in their respective cells - DONE
	//NOTE: Each cell goes from 0 to less than their CELL_SIZE value. That means that if CELL_SIZE is 1, it goes from 0 to 0.9999999(9), and not 0 to 1.
	/** /
	int number = 0;
	int count = 0;
	for (std::vector<Cell>::iterator cell = _grid.begin(); cell != _grid.end(); cell++){
	std::cout << "In Cell " << number << " there is a total of " << cell->objects.size() << " objects:" << std::endl;
	number++;
	if (cell->objects.size() > 0)
	count++;
	//for (std::vector<Object*>::iterator it = cell->objects.begin(); it != cell->objects.end(); it++){
	//	std::cout << typeid(*it).name() << std::endl;
	//}
	}
	std::cout << "Ocuppied cells number: " << count << std::endl;
	/**/

	//DEBUG - Verify if planes are removed from everything - DONE
	/** /
	std::cout << "Objects in total: " << objs.size() << std::endl;
	std::cout << "Objects without planes involved: " << objsGuarded << std::endl;
	/**/
}

bool RegularGrid::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &tMin, glm::vec3 &tMax){

	float tmin, tmax;
	float tprox = -(float)INFINITE;
	float tdist = (float)INFINITE;

	glm::vec3 v0 = ray.O;
	glm::vec3 vd = ray.D;
	glm::vec3 vmin = _min;
	glm::vec3 vmax = _max;

	//Calcular o as intercessoes dos planos x, y e z
	for (int i = 0; i < 3; i++){

		//Se vd for nulo o raio e paralelo aos planos e se estiver fora do plano naon exister intercessao
		if (vd[i] == 0 && (v0[i] < vmin[i] || v0[i] > vmax[i]))
			return false;

		//tmin e tmax para o plano respectivo
		tmin = (vmin[i] - v0[i]) / vd[i];
		tmax = (vmax[i] - v0[i]) / vd[i];

		if (tmin > tmax)
			SWAP(float, tmin, tmax);

		if (tmin > tprox)
			tprox = tmin;

		if (tmax < tdist)
			tdist = tmax;

		//Se tprox > tdist o raio nao intersecta e se tdist < 0 o raio aponta na direccao contraria
		if (tprox > tdist || tdist < 0)
			return false;

		tMin[i] = tmin;
		tMax[i] = tmax;
	}

	point = glm::vec3(ray.O.x + ray.D.x * tprox, ray.O.y + ray.D.y * tprox, ray.O.z + ray.D.z * tprox);

	return true;
}

bool RegularGrid::isRayInsideGrid(Ray ray){
	return (ray.O.x < _max.x && ray.O.x > _min.x && ray.O.y < _max.y && ray.O.y > _min.y && ray.O.z < _max.z && ray.O.z > _min.z);
}

std::vector<Object*> RegularGrid::traversalAlgorithm(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	std::vector<Object*> final_objects;
	glm::vec3 p, tmin, tmax;

	if (!rayInterception(ray, p, tmin, tmax)) return final_objects;

	//Traverse Algorithm
	glm::vec3 pf = isRayInsideGrid(ray) ? ray.O : p;
	glm::vec3 tNext, step, stop, dt;
	int ixyz[3];

	//Actualizar o variaveis de acordo com a direccao do raio
	for (int i = 0; i < 3; i++){
		//Calcular ponto de interseccao na grid, usando o point
		ixyz[i] = (int)CLAMP((pf[i] - _min[i]) * _N[i] / (_max[i] - _min[i]), 0, _N[i] - 1);

		dt[i] = (tmax[i] - tmin[i]) / (float)_N[i];

		if (ray.D[i] > 0){
			tNext[i] = tmin[i] + (ixyz[i] + 1) * dt[i];
			step[i] = 1.0f;
			stop[i] = (float)_N[i];
		}
		else{
			tNext[i] = tmax[i] - ixyz[i] * dt[i];
			step[i] = -1.0f;
			stop[i] = -1.0f;
		}

		//Caso seja paralelo a um eixo o next e infinito
		if (ray.D[i] == 0.0f)
			tNext[i] = (float)INFINITE;
	}

	float tB = (float)INFINITE;
	float t;
	Object *oB = NULL;
	glm::vec3 bPoint, bNormal;

	//Econtrar o objecto mais proximo caso exista e retornar
	while (true){

		//Proxima celula
		std::vector<Object*> objs = _grid[ixyz[0] + _N[0] * ixyz[1] + _N[0] * _N[1] * ixyz[2]].objects;

		//Procura o objecto mais proximo onde exista interseccao
		for (Object* obj : objs){
			if (!Utils::equalRay(ray, obj->getLastInterception(glm::vec3(), glm::vec3()))){
				if (obj->rayInterception(ray, bPoint, bNormal, t) && t < tB){
					oB = obj;
					tB = t;
					point = bPoint;
					normal = bNormal;
				}
			}
		}

		int i = (tNext.x < tNext.y && tNext.x < tNext.z) ? 0 : ((tNext.y < tNext.z) ? 1 : 2);

		//Se encontrou um objecto termina
		if (oB != NULL && tB < tNext[i]){
			final_objects.push_back(oB);
			return final_objects;
		}

		//Avanca
		ixyz[i] += (int)step[i];
		tNext[i] += dt[i];
		//Se chegou ao limite termina
		if (ixyz[i] == (int)stop[i]) return final_objects;
	}


}