#pragma once

#include "RayTracing.h"

class Sampling
{
	public:
		static glm::vec3 recursiveFill(glm::vec2 subpixel, int recursive, std::vector<std::vector<glm::vec3>> &buffer);
};

