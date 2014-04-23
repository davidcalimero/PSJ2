#pragma once

#include "Scene.h"


class RayTracing {
	private:
		static bool isAffectedByLight(Ray ray);
		static glm::vec3 calculateShadow(glm::vec3 lighPosition, glm::vec3 lighColor, glm::vec3 point, glm::vec3 normal, Object *oB);
		static glm::vec3 shade(Object * oB, glm::vec3 normal, glm::vec3 point);
		static Object * nearestIntersection(Ray ray);

		static void calculateReflection(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color);
		static void calculateRefraction(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color);
		
	public:
		static glm::vec3 rayTracing(Ray ray, int depth, float ior);
};