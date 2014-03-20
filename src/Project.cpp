
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <iomanip>

//#include "scene.h"


#define MAX_DEPTH 6
//Scene* scene = NULL;
int RES_X, RES_Y;
float loading = 0;
int WindowHandle = 0;

void loading_print();

void reshape(int w, int h)
{
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
void drawScene()
{
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			//Ray ray = scene->GetCamera()->PrimaryRay(x, y);
			//Color color = rayTracing(ray, 1, 1.0); //depth=1, ior=1.0

			loading = (x+1 + y*(RES_X)) / (float)(RES_X * RES_Y) * 100.0f;
			loading_print();

			glBegin(GL_POINTS);
			glColor3f(1, 0, 0);
			glVertex2f(x, y);
			glEnd();
			glFlush();
		}
	}
	
	printf("Terminou!\n");
}

void loading_print() {
	std::ostringstream oss;
	oss << std::setprecision(2) << std::fixed;
	oss << "FRED Ray Tracing: " << loading << "%";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
}

int main(int argc, char**argv)
{
	//scene = new Scene();
	//if (!(scene->load_nff("jap.nff"))) return 0;
	RES_X = 500;//scene->GetCamera()->GetResX();
	RES_Y = 500;//scene->GetCamera()->GetResY();
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);
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