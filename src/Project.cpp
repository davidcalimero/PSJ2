#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <glm.hpp>
#include <vector>
#include <thread>

#include "Scene.h"

#define MAX_DEPTH 6

Scene* scene;
int RES_X, RES_Y;
float Loading = 0;
int WindowHandle = 0;
float matrix[512][512][3];

glm::vec3 rayTracing(Ray ray, int depth, int ior);

void paint(int xi, int yi, int w, int h)
{
	for (int y = yi; y < yi+h; y++) {
		for (int x = xi; x < xi+w; x++) {
			Ray ray = scene->GetCamera()->PrimaryRay(x, y);
			glm::vec3 color = rayTracing(ray, 1, 1);
			matrix[y][x][0] = color.x;
			matrix[y][x][1] = color.y;
			matrix[y][x][2] = color.z;
		}
	}
}

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
	std::thread t1(paint, 0, 0, 64, 64);
	std::thread t2(paint, 64, 0, 64, 512);
	std::thread t3(paint, 128, 0, 64, 512);
	std::thread t4(paint, 192, 0, 64, 512);
	std::thread t5(paint, 256, 0, 64, 512);
	std::thread t6(paint, 320, 0, 64, 512);
	std::thread t7(paint, 384, 0, 64, 512);
	std::thread t8(paint, 448, 0, 64, 512);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {

			Loading = (x + 1 + y*(RES_X)) / (float)(RES_X * RES_Y) * 100.0f;
			loading_print();

			glBegin(GL_POINTS);
			glColor3f(matrix[y][x][0], matrix[y][x][1], matrix[y][x][2]);
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


bool isAffectedByLight(Ray ray){
	std::vector<Object*> objs = scene->GetObjects();
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		glm::vec3 normal;
		if (((Object*)(*it))->rayInterception(ray, point, normal)){
			return false;
		}
	}
	return true;
}


//Ray Tracing

glm::vec3 rayTracing(Ray ray, int depth, int ior){

	//Determinar interseção mais proxima com um objecto
	std::vector<Object*> objs = scene->GetObjects();
	float nearestPoint = -1;
	glm::vec3 color = scene->GetBckgColor();

	glm::vec3 pointB;
	glm::vec3 normalB;
	Object* oB;

	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		glm::vec3 normal;
		bool intercept = ((Object*)(*it))->rayInterception(ray, point, normal);

		float dist = glm::distance(ray.O, point);
		if ((nearestPoint == -1 || dist < nearestPoint) && intercept){
			nearestPoint = dist;
			color = ((Object*)(*it))->GetFillColor();
			pointB = point;
			normalB = normal;
			oB = (Object*)(*it);
		}


	}

	//Calcular Raios Sombra

	std::vector<Light*> lights = scene->GetLights();
	if (lights.size() > 0) color = glm::vec3(0);
	for (std::vector<Light*>::iterator il = lights.begin(); il != lights.end(); il++){
		glm::vec3 L = glm::normalize((*il)->position - pointB);
		if (glm::dot(L, normalB) > 0){
			Ray shadow;
			shadow.O = pointB + 0.001f*L;
			shadow.D = L;
			if (isAffectedByLight(shadow)){
				//color += (*il)->color * (*it)->Get_k_constants().x;// + glm::vec3(1) * (*it)->Get_k_constants().y * (*it)->Get_k_constants().z;
				//color -= (*it)->Get_k_constants().x + (*it)->Get_k_constants().y;
				//color += (*il)->color * oB->Get_k_constants().x + (*il)->color * oB->Get_k_constants().y * oB->Get_k_constants().z;
				glm::vec3 H = glm::normalize(L + glm::normalize(scene->GetCamera()->GetPos() - pointB));
				color += oB->Get_k_constants().x * (*il)->color * oB->GetFillColor() * glm::dot(L, normalB) + oB->Get_k_constants().y * (*il)->color * oB->GetFillColor() * pow(glm::dot(H, normalB), oB->Get_k_constants().z);

			}
		}
	}

	return color;
}