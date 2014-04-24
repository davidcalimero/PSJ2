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


// NOME DO FICHEIRO NFF
#define NFF_FILENAME "scenes/demo_ply.nff"
//--------------------------


// CONFIGURACOES DO PROJECTO
//Threads
#define N_THREADS 1			 //Numero de Threads

//Raytracing
#define MAX_DEPTH 6		     //Profundidade dos raios secundarios

//Light
#define LIGHTS_AREA 1.0f     //Raio da area ocupada pela luz
#define LIGHT_SAMPLE 1.0f    //Numero de amostras de luzes (LIGHT_SAMPLE X LIGHT_SAMPLE)

//Sampling
#define COLOR_THRESHOLD 0.3f //Diferenca de cor aceitavel no sampling
#define MAX_SAMPLING 0	     //n x anti-aliasing (0 = no anti-aliasing)

//Grid
#define M 2.0f				 //Factor de maximizacao

//DOF
#define FOCALLENGTH 2.5f	 //Distancia focal da lente do DOF
#define N_DEPTH_RAYS 0      //Numero de raios enviados pelo DOF num pixel (0 = no DOF)
//--------------------------


//TGA -------------------
#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10
//--------------------------


//MACROS -------------------
#define PI 3.14159265
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define CLAMP(a,min,max) (MAX(MIN(a,max),min))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}
#define PRINT(text) std::cout << text << std::endl;
//--------------------------


//ESTRUTURAS -------------
typedef GLushort WORD;
typedef GLubyte byte;

typedef struct {
	glm::vec3 O;
	glm::vec3 D;
} Ray;

typedef struct tImageTGA
{
	int channels;
	int size_x;
	int size_y;
	unsigned char *data;
} tImageTGA;

typedef struct {
	glm::vec3 fill_color;
	glm::vec3 k_constants;
	float transmittance;
	float indexRefraction;
	tImageTGA *texture;
} Properties;

typedef struct {
	glm::vec3 position;
	glm::vec3 color;
} Light;

typedef struct {
	glm::vec3 pmin;
	glm::vec3 pmax;
} BoundingBox;

typedef struct {
	GLfloat XYZ[3];
	GLfloat N[3];
	GLfloat UV[2];
} Vertex;

typedef struct Face {
	unsigned int count;
	unsigned int *vertices;
	GLfloat N[3];
	GLfloat UV[2];
} Face;
//--------------------------


//UTILS --------------------
namespace Utils {

	float norma(glm::vec3 v);
	tImageTGA *loadTexture(const char *strfilename);
	bool equalRay(Ray r1, Ray r2);
}
//--------------------------
