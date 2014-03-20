#pragma once

#include <glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#define PI 3.14159265

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}

typedef struct {
	glm::vec3 O;
	glm::vec3 D;
} Ray;


typedef struct {
	glm::vec3 fill_color;
	glm::vec3 k_constants;
	float transmittance;
	float indexRefraction;
} Properties;

/** /
typedef struct {
	glm::vec3 p1, p2, p3;
	Properties properties;
} Triangle;


typedef struct {
	glm::vec3 center;
	float radius;
	Properties properties;
} Sphere;
/**/

typedef struct {
	glm::vec3 position;
	glm::vec3 color;
} Light;


namespace Utils {

	float norma(glm::vec3 v);
	//bool loadNFF(char* filename, glm::vec3 &from, glm::vec3 &at, glm::vec3 &up, float &angle, float &hither, glm::vec2 &resolution, glm::vec3 &bckg_color, std::vector<Light*> &lights, std::vector<Object*> &objects);
}
