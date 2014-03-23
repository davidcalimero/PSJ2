#include "Utils.h"

namespace Utils {

	//Calcula a norma de um vector
	float norma(glm::vec3 v){
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
}
