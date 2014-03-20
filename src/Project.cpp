#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <glm.hpp>
#include <vector>

#include "Scene.h"

#define MAX_DEPTH 6

Scene* scene;
int RES_X, RES_Y;
float Loading = 0;
int WindowHandle = 0;

glm::vec3 rayTracing(Ray ray, int depth, int ior);

//Imprime no cabecalho da janela a percentagem de carregamento da imagem
void loading_print() {
	std::ostringstream oss;
	oss << std::setprecision(2) << std::fixed;
	oss << "FRED Ray Tracing: " << Loading << "%";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
}


void reshape(int w, int h) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, RES_X - 1, 0, RES_Y - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Draw function by primary ray casting from the eye towards the scene's objects
void drawScene() {
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			Ray ray = scene->GetCamera()->PrimaryRay(x, y);
			//Color color = rayTracing(ray, 1, 1.0); //depth=1, ior=1.0
			glm::vec3 color = rayTracing(ray, 1, 1);

			Loading = (x + 1 + y*(RES_X)) / (float)(RES_X * RES_Y) * 100.0f;
			loading_print();

			glBegin(GL_POINTS);
			glColor3f(color.x, color.y, color.z);
			glVertex2f(x, y);
			glEnd();
			glFlush();
		}
	}

	std::cout << "Terminou!" << std::endl;
}


int main(int argc, char**argv) {

	scene = new Scene();
	if (!(scene->loadNFF("test.nff"))) return 0;
	RES_X = scene->GetCamera()->GetResX();
	RES_Y = scene->GetCamera()->GetResY();
	std::cout << "Resolution: " << RES_X << " X " << RES_Y << std::endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitWindowPosition(100, 100);
	WindowHandle = glutCreateWindow("");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}



//Ray Tracing

glm::vec3 rayTracing(Ray ray, int depth, int ior){
	std::vector<Object*> objs = scene->GetObjects();

	float nearestPoint = -1;
	glm::vec3 color;
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		bool intercept = ((Object*)(*it))->rayInterception(ray, point);

		float dist = glm::distance(ray.O, point);
		if ((nearestPoint == -1 || dist < nearestPoint) && intercept){
			nearestPoint = dist;
			color = ((Object*)(*it))->GetFillColor();
		}
	}

	if (nearestPoint != -1)
		return color;

	return scene->GetBckgColor();
}