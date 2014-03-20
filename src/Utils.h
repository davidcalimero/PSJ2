#pragma once

#include <glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}

typedef struct {
	glm::vec3 fill_color;
	glm::vec3 k_constants;
	float transmittance;
	float indexRefraction;
} Properties;


typedef struct {
	glm::vec3 p1, p2, p3;
	Properties properties;
} Polygon;


typedef struct {
	glm::vec3 center;
	float radius;
	Properties properties;
} Sphere;


typedef struct {
	glm::vec3 position;
	glm::vec3 color;
} Light;

namespace Utils {

	void loadNFF(char* filename);
}
