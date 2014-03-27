#include "Triangle.h"

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction) {
	_a = v1;
	_b = v2;
	_c = v3;

	_normal = -glm::normalize(glm::cross(_c - _a, _b - _a));

	//Mostra se o triangle foi bem criado (DEBUG)
	/** /
	std::cout << "Triangle created with " << 3 << " vertexes: " << std::endl;
	std::cout << "(" << v1.x << ", " << v1.y << ", " << v1.z << ") " << std::endl;
	std::cout << "(" << v2.x << ", " << v2.y << ", " << v2.z << ") " << std::endl;
	std::cout << "(" << v3.x << ", " << v3.y << ", " << v3.z << ") " << std::endl;

	std::cout << "(" << _normal.x << ", " << _normal.y << ", " << _normal.z << ") << Normal" << std::endl;
	/**/
}


bool Triangle::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	// Ray defined by:
	// p(t) = o + d*t 

	// Triangle defined by:
	// p(B,Y) = a + B(b-a) + Y(c-a)

	// o + d*t = a + B(b-a) + Y(c-a)
	// ...
	// B(b-a) + Y(c-a) + t(-d) = o - a

	// [(b-a) (c-a) -d]*[B Y t]^T = (o - a)    (*) ^T - Transpose

	glm::vec3 d = ray.D;
	glm::vec3 o = ray.O;

	glm::vec3 v1 = _b - _a;
	glm::vec3 v2 = _c - _a;
	glm::vec3 v3 = -d;
	glm::vec3 vs = o - _a;

	// [v1 v2 v3]*[B Y t]^T = vs

	// B = |vs v2 v3|/|v1 v2 v3|

	// B = |vs.x v2.x v3.x|   |v1.x v2.x v3.x|
	//     |vs.y v2.y v3.y| / |v1.x v2.x v3.x|
	//     |vs.z v2.z v3.z|   |v1.x v2.x v3.x|

	//     vs.x * (v2.y*v3.z - v3.y*v2.z) + v2.x * (v3.y*vs.z - vs.y*v3.z) + v3.x * (vs.y*v2.z - v2.y*vs.z)
	// B = -------------------------------------------------------------------------------------------------
	//     v1.x * (v2.y*v3.z - v3.y*v2.z) + v2.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*v2.z - v2.y*v1.z)

	float denom = v1.x * (v2.y*v3.z - v3.y*v2.z) + v2.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*v2.z - v2.y*v1.z);

	float B = (vs.x * (v2.y*v3.z - v3.y*v2.z) + v2.x * (v3.y*vs.z - vs.y*v3.z) + v3.x * (vs.y*v2.z - v2.y*vs.z)) / denom;

	// Y = |v1 vs v3|/|v1 v2 v3|

	// Y = |v1.x vs.x v3.x|   |v1.x v2.x v3.x|
	//     |v1.y vs.y v3.y| / |v1.x v2.x v3.x|
	//     |v1.z vs.z v3.z|   |v1.x v2.x v3.x|

	//     v1.x * (vs.y*v3.z - v3.y*vs.z) + vs.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*vs.z - vs.y*v1.z)
	// Y = -------------------------------------------------------------------------------------------------
	//     v1.x * (v2.y*v3.z - v3.y*vs.z) + v2.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*v2.z - v2.y*v1.z)

	float Y = (v1.x * (vs.y*v3.z - v3.y*vs.z) + vs.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*vs.z - vs.y*v1.z)) / denom;

	// t = |v1 v2 vs|/|v1 v2 v3|

	// t = |v1.x v2.x vs.x|   |v1.x v2.x v3.x|
	//     |v1.y v2.y vs.y| / |v1.x v2.x v3.x|
	//     |v1.z v2.z vs.z|   |v1.x v2.x v3.x|

	//     v1.x * (v2.y*vs.z - vs.y*v2.z) + v2.x * (vs.y*v1.z - v1.y*vs.z) + vs.x * (v1.y*v2.z - v2.y*v1.z)
	// t = -------------------------------------------------------------------------------------------------
	//     v1.x * (v2.y*v3.z - v3.y*vs.z) + v2.x * (v3.y*v1.z - v1.y*v3.z) + v3.x * (v1.y*v2.z - v2.y*v1.z)

	float ti = (v1.x * (v2.y*vs.z - vs.y*v2.z) + v2.x * (vs.y*v1.z - v1.y*vs.z) + vs.x * (v1.y*v2.z - v2.y*v1.z)) / denom;

	if (ti < 0) return false;

	if (B < 0 || B > 1 || Y < 0 || Y > 1 || (B+Y) > 1)
		return false;

	normal = _normal;
	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);

	return true;
}