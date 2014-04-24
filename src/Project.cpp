#include "Sampling.h"


int RES_X, RES_Y;  //Resolucao do ecra
std::vector<std::vector<glm::vec3>> buffer; //Buffer onde as threads vao pintar - representa as cores DOS CANTOS dos pixeis
std::vector<std::thread> threads; //Threads que vao ser utilizadas


//Funcao utilizada pelas threads
void sendRay(int xi, int xf, int yi, int yf) {
	for (int y = yi; y < yf; y++)
		for (int x = xi; x < xf; x++)
			buffer[y][x] = Sampling::recursiveFill(glm::vec2(x, y), 1);
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
	for (int i = 0; i < n_threads; i++){
		threads[i].join();
		threads[i].~thread();
	}
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
	auto start = std::chrono::steady_clock::now();
	
	/**/ //As threads vao actualizar o buffer com as respectivas cores
	createThreadsAndJoin();

	//Ciclo principal de pintura
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//Pinta o pixel
			glBegin(GL_POINTS);
			glColor3f(buffer[y][x].r, buffer[y][x].g, buffer[y][x].b);
			glVertex2f((GLfloat)x, (GLfloat)y);
			glEnd();
		}
	}
	/**/
	
	glFlush();
	
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	PRINT("It took me " << elapsed.count() / 1000000.0f << " seconds and "  << Scene::getInstance().getNumberOfInterceptions() << " interceptions!");
}


//Key Handler
void keyUp(unsigned char key, int x, int y){
	if (key == 'G' || key == 'g'){
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		Scene::getInstance().toggleGrid();
		glutPostRedisplay();
	}
}


//MAIN
int main(int argc, char**argv) {

	//Se nao conseguir ler o ficheiro termina
	if (!(Scene::getInstance().loadNFF(NFF_FILENAME))) return 0;

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
	glutKeyboardUpFunc(keyUp);
	glDisable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}