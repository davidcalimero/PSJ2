#include "Utils.h"

namespace Utils {

	float norma(glm::vec3 v){
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
}
