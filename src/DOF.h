#pragma once

#include "RayTracing.h"

class DOF
{
	public:
		static glm::vec3 DepthOfField(glm::vec2 pixel, float focalLength);
};
