#include "Sampling.h"
#include "DOF.h"
#include "RegularGrid.h"


int RES_X, RES_Y; //resolucao do ecra
std::vector<std::vector<glm::vec3>> buffer; //buffer onde as threads vao pintar - representa as cores DOS CANTOS dos pixeis
std::vector<std::thread> threads; //threads que vao ser utilizadas


//Funcao utilizada pelas threads
void sendRay(int xi, int xf, int yi, int yf) {
	for (int y = yi; y < yf; y++){
		for (int x = xi; x < xf; x++){
			if (DOF_ACTIVE){
				buffer[y][x] = DOF::DepthOfField(glm::vec2(x, y));
			}
			else{
				buffer[y][x] = Sampling::recursiveFill(glm::vec2(x, y), 1);
			}
		}
	}
}


//Cria N_THREADS que vao utilizar a funcao sendRay e faz join a cada uma delas
void createThreadsAndJoin(){
	//Limpa o vector de threads
	threads.clear();

	//Cria as threads dividindo a janela por cada thread
	int n_threads = MIN(N_THREADS, RES_X);
	int inicio = (int)floor(RES_X / (float)n_threads);
	for (int i = 0; i < n_threads; i++){
		int fim = i * inicio + inicio;
		if (i == n_threads - 1) fim = RES_X;
		//std::cout << "thread " << i << " inicio: " << i*inicio << " fim: " << fim << std::endl;
		threads.push_back(std::thread(sendRay, i * inicio, fim, 0, RES_Y));
	}

	//Faz join a cada uma das threads
	for (int i = 0; i < n_threads; i++)
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
			//Pinta o pixel
			glBegin(GL_POINTS);
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

	//Criacao da grid onde os objectos vao ficar
	RegularGrid* grid = new RegularGrid(Scene::getInstance().GetObjects());

	//Actualiza resolucao da janela
	RES_X = Scene::getInstance().GetCamera()->GetResX();
	RES_Y = Scene::getInstance().GetCamera()->GetResY();

	//Resize Buffer
	buffer.resize(RES_Y);
	for (int i = 0; i < RES_Y; ++i)
		buffer[i].resize(RES_X);

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