#pragma once

#include "Object.h"

class AABB : public Object {
	private:

	public:
		AABB(glm::vec3 p1, glm::vec3 p2, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);
		AABB(glm::vec3 p1, glm::vec3 p2);
		virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);
};

