#pragma once

#include "Object.h"

class Poly : public Object {

	private:
		int _nVertices;
		std::vector<glm::vec3> _vertices;
		glm::vec3 _normal;

	public:
		Poly(int nVertices, std::vector<glm::vec3> vertices, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction);
		virtual bool rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal);
		bool Poly::windingNumber(Ray ray, glm::vec3 &p, glm::vec3 &n);
		bool returnPoint(Ray ray, glm::vec3 &point, glm::vec3 &normal);
		float isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2);
};

