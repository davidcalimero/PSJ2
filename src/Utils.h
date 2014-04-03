#pragma once

#include <GL/glut.h>
#include <glm.hpp>
#include <iomanip>
#include <vector>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#define MAX_DEPTH 6
#define N_THREADS 8
#define LIGHTS_AREA 8.0f  //LIGHTS_AREA X LIGHTS_AREA

#define LIGHT_TYPE true  //true -> Luz em area   false -> Luz em ponto

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


typedef struct {
	glm::vec3 position;
	glm::vec3 color;
} Light;


namespace Utils {

	float norma(glm::vec3 v);
}
