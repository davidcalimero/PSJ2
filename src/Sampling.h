#pragma once

#include "DOF.h"

class Sampling
{
	public:
		static glm::vec3 recursiveFill(glm::vec2 subpixel, int recursive);
};

