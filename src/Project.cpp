#include "Sampling.h"
#include "RegularGrid.h"


int RES_X, RES_Y; //resolucao do ecra
std::vector<std::vector<glm::vec3>> buffer; //buffer onde as threads vao pintar - representa as cores DOS CANTOS dos pixeis
std::vector<std::thread> threads; //threads que vao ser utilizadas


//Enviar um raio
void sendRay(int xi, int yi, int w, int h) {
	for (int y = yi; y < yi + h; y++) {
		for (int x = xi; x < xi + w; x++) {
			Ray ray = Scene::getInstance().GetCamera()->PrimaryRay(x - 0.5f, y - 0.5f);
			buffer[y][x] = RayTracing::rayTracing(ray, 1, 1);
		}
	}
}


//Cria N_THREADS que vao utilizar a funcao sendRay e faz join a cada uma delas
void createThreadsAndJoin(){
	//Limpa o vector de threads
	threads.clear();

	//Cria as threads
	for (int i = 0; i < N_THREADS; i++)
		threads.push_back(std::thread(sendRay, i * (RES_X + 1) / N_THREADS, 0, (RES_X + 1) / N_THREADS, RES_Y));

	//Faz join
	for (int i = 0; i < N_THREADS; i++)
		threads[i].join();
}


//Reshape
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
	time_t timeBefore, timeAfter;
	time(&timeBefore);

	//As threads vao actualizar o buffer com as respectivas cores
	createThreadsAndJoin();

	//Sampling
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//Calcula a cor do pixel
			//glm::vec3 color = Sampling::recursiveFill(glm::vec2(x,y), 1, buffer);
			//Pinta o pixel
			glBegin(GL_POINTS);
			//glColor3f(color.r, color.g, color.b);
			glColor3f(buffer[y][x].r, buffer[y][x].g, buffer[y][x].b);
			glVertex2f((GLfloat)x, (GLfloat)y);
			glEnd();
		}
	}

	glFlush();
	time(&timeAfter);
	std::cout << "Render time: " << difftime(timeAfter, timeBefore) << " seconds!" << std::endl;
}


//MAIN
int main(int argc, char**argv) {

	//Se nao conseguir ler o ficheiro termina
	if (!(Scene::getInstance().loadNFF("scenes/balls_low.nff"))) return 0;

	RegularGrid* grid = new RegularGrid(Scene::getInstance().GetObjects());

	//Actualiza resolucao da janela
	RES_X = Scene::getInstance().GetCamera()->GetResX();
	RES_Y = Scene::getInstance().GetCamera()->GetResY();

	//Resize Buffer
	buffer.resize(RES_Y + 1);
	for (int i = 0; i < RES_Y + 1; ++i) {
		buffer[i].resize(RES_X + 1);
	}

	std::cout << "Resolution: " << RES_X << " X " << RES_Y << std::endl;

	//Inicializacoes do OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("FRED Ray Tracing");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}