#pragma once

#include <GL/glut.h>
#include <glm.hpp>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <vector>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>


// CONFIGURACOES DO PROJECTO
#define N_THREADS 10         //Numero de Threads
#define MAX_DEPTH 6		     //Profundidade dos raios secundarios
#define LIGHTS_AREA 1.0f     //Area ocupada por uma luz (LIGHTS_AREA X LIGHTS_AREA)
#define COLOR_THRESHOLD 0.3  //DiferenCa de cor aceitavel no sampling
#define MAX_SAMPLING 0	     //n x anti-alising (0 = no anti-aliasing)
#define CELL_SIZE 1.0f       //max size of cells
//--------------------------


//MACROS -------------------
#define PI 3.14159265
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define CLAMP(a,min,max) (MAX(MIN(a,max),min))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}
//--------------------------


//ESTRUTURAS -------------
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

typedef struct {
	glm::vec3 pmin;
	glm::vec3 pmax;
} BoundingBox;
//--------------------------


//UTILS --------------------
namespace Utils {

	float norma(glm::vec3 v);
}
//--------------------------
