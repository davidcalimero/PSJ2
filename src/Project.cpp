#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <glm.hpp>
#include <vector>
#include <thread>

#include "Scene.h"

#define MAX_DEPTH 2
#define N_THREADS 1

std::ofstream myfile;
Scene* scene;
int RES_X, RES_Y; //resolucao do ecra
float Loading = 0; //% de carregamento
int WindowHandle = 0; //id da janela
std::vector<std::vector<glm::vec3>> buffer; //buffer onde as threads vao pintar
std::vector<std::thread> threads; //threads que vao ser utilizadas

glm::vec3 rayTracing(Ray ray, int depth, int ior);

void sendRay(int xi, int yi, int w, int h)
{
	myfile.open("example.txt");
	for (int y = yi; y < yi+h; y++) {
		for (int x = xi; x < xi+w; x++) {
			Ray ray = scene->GetCamera()->PrimaryRay(x, y);
			buffer[y][x] = rayTracing(ray, 1, 1);
			if (buffer[y][x].x < 0.2 && buffer[y][x].y < 0.2 && buffer[y][x].z < 0.2)
				myfile << "in point (" << x << "," << y << "), there is a color close to black, that is: (" << buffer[y][x].x << "," << buffer[y][x].y << "," << buffer[y][x].z << ")" << std::endl;
		}
	}
	myfile.close();
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

//Cria N_THREADS threads que vao utilizar a funcao sendRay e faz join a cada uma delas
void createThreadsAndJoin(){
	//Limpa o vector de threads
	threads.clear();	
	
	//Cria as threads
	for (int i = 0; i < N_THREADS; i++)
		threads.push_back(std::thread(sendRay, i * RES_X / N_THREADS, 0, RES_X / N_THREADS, RES_Y));

	//Faz join
	for (int i = 0; i < N_THREADS; i++)
		threads[i].join();
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

	//As threads vao actualizar o buffer com as respectivas cores
	createThreadsAndJoin();

	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//Actualiza a % de carregamento
			Loading = (x + 1 + y*(RES_X)) / (float)(RES_X * RES_Y) * 100.0f;
			loading_print();

			//Pinta o pixel
			glBegin(GL_POINTS);
			glColor3f(buffer[y][x].x, buffer[y][x].y, buffer[y][x].z);
			glVertex2f((GLfloat)x, (GLfloat)y);
			glEnd();
			
		}
		glFlush();
	}
	
	std::cout << "Terminou!" << std::endl;
}


int main(int argc, char**argv) {
	
	scene = new Scene();
	//Se nao conseguir ler o ficheiro termina
	if (!(scene->loadNFF("scenes/simpleTest.nff"))) return 0;
	
	//Actualiza resolucao da janela
	RES_X = scene->GetCamera()->GetResX();
	RES_Y = scene->GetCamera()->GetResY();

	//Resize buffer
	buffer.resize(RES_Y);
	for (int i = 0; i < RES_Y; ++i) {
		buffer[i].resize(RES_X);
	}

	std::cout << "Resolution: " << RES_X << " X " << RES_Y << std::endl;
	
	//Inicializacoes do OpenGL
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

//Verifica se um raio em direcao a cada fonte de luz tem algum objecto pelo meio
bool isAffectedByLight(Ray ray){
	std::vector<Object*> objs = scene->GetObjects();
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		glm::vec3 normal;
		//Se existir uma intercessao esse ponto nao e afectado directamente pela luz
		if (((Object*)(*it))->rayInterception(ray, point, normal)){
			return false;
		}
	}
	return true;
}


//Ray Tracing
glm::vec3 rayTracing(Ray ray, int depth, int ior){

	std::vector<Object*> objs = scene->GetObjects();
	float nearestPoint = -1;
	glm::vec3 color = scene->GetBckgColor();

	glm::vec3 pointB;
	glm::vec3 normalB;
	Object* oB;

	//Determinar interseção mais proxima com um objecto
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		glm::vec3 normal;
		bool intercept = ((Object*)(*it))->rayInterception(ray, point, normal);
		float dist = glm::distance(ray.O, point);

		//Se estiver mais proximo actulizar valores
		if ((nearestPoint == -1 || dist < nearestPoint) && intercept){
			nearestPoint = dist;
			color = ((Object*)(*it))->GetFillColor();
			pointB = point;
			normalB = normal;
			oB = (Object*)(*it);

			if (color.x < 0.1 && color.y < 0.1 && color.z < 0.1){
				myfile << "deu erro no fillcolor" << std::endl;
			}
		}
	}

	if (nearestPoint == -1) return color;

	//Calcular Raios Sombra
	std::vector<Light*> lights = scene->GetLights();
	if (lights.size() > 0) color = glm::vec3(0);
	for (std::vector<Light*>::iterator il = lights.begin(); il != lights.end(); il++){
		glm::vec3 L = glm::normalize((*il)->position - pointB);
		if (glm::dot(L, normalB) > 0){
			Ray shadow;
			//Margem de erro para o caso do raio trespassar a esfera 
			shadow.O = pointB + 0.001f*L;
			shadow.D = L;

			//Se nao existir um objecto em direcao a luz, calcular a cor do ponto com a respectiva luz
			if (isAffectedByLight(shadow)){
				glm::vec3 H = glm::normalize(L + glm::normalize(scene->GetCamera()->GetPos() - pointB));
				//glm::vec3 R = 2 * glm::dot(-ray.D,normalB)* normalB + ray.D;
				color = color + (oB->Get_k_constants().x * (*il)->color * oB->GetFillColor() * glm::dot(L, normalB)) + 
								(oB->Get_k_constants().y * (*il)->color * oB->GetFillColor() * pow(glm::dot(H, normalB), oB->Get_k_constants().z));
			}
			if (color.x <= 0 || color.y <= 0 || color.z <= 0){
				myfile << "deu erro quando calcula a cor com a luz" << std::endl;
				myfile << color.x << " " << color.y << " " << color.z << std::endl;
			}
		}
		else if (color.x <= 0 || color.y <= 0 || color.z <= 0){
			myfile << "deu erro se o dot > 0" << std::endl;
			myfile << color.x << " " << color.y << " " << color.z << std::endl;
		}
	}

	/**/
	if (color.x <= 0 || color.y <= 0 || color.z <= 0){
		myfile << "deu erro depois da luz" << std::endl;
		myfile << color.x << " " << color.y << " " << color.z << std::endl;
	}
	/**/		

	// Verificar se esta no depth maximo
	if (depth >= MAX_DEPTH) return color;

	// Calcular Raios de Reflexao
	/**/
	if (oB->Get_k_constants().y != 0){
		glm::vec3 E = ray.D;
		glm::vec3 R = E - (2 * glm::dot(E, normalB) * normalB);
		Ray reflected_ray;
		reflected_ray.O = pointB + 0.001f*R;
		reflected_ray.D = R;
		glm::vec3 reflected_color;
		reflected_color = rayTracing(reflected_ray, depth + 1, ior);

		reflected_color *= oB->Get_k_constants().y;
		color += reflected_color;
		if (color.x < 0.1 && color.y < 0.1 && color.z < 0.1){
			myfile << "deu erro depois da reflexao no depth " << depth+1 << std::endl;
			myfile << reflected_color.x << " " << reflected_color.y << " " << reflected_color.z << std::endl;
		}
	}
	/** /
	if (color.x < 0.1 && color.y < 0.1 && color.z < 0.1){
		myfile << "deu erro depois da reflexao" << std::endl;
		myfile << color.x << " " << color.y << " " << color.z << std::endl;
	}
	/**/

	/**/
	// Calcular Raios de Refraccao
	if (oB->getTransmittance() != 0){
		// Ver questão do sinal do ray.D
		glm::vec3 vt = glm::dot(-ray.D, normalB) * normalB + ray.D;
		float sin_teta_i = Utils::norma(vt);
		// Ver se está dentro ou fora do objecto
		float sin_teta_t;
		float new_reflected_index;
		if (ior != 1){ //dentro do objecto
			new_reflected_index = 1; 
			sin_teta_t = ior * sin_teta_i;
		}
		else {
			new_reflected_index = oB->getRefractionIndex();
			sin_teta_t = ior / new_reflected_index * sin_teta_i;
		}
		float cos_teta_t = sqrt(1 - (sin_teta_t * sin_teta_t));
		glm::vec3 t = glm::normalize(vt);
		glm::vec3 rt = sin_teta_t*t + cos_teta_t * (-normalB);
				
		Ray refracted_ray;
		refracted_ray.O = pointB + 0.001f*rt;
		refracted_ray.D = rt;
		glm::vec3 refracted_color;
		refracted_color = rayTracing(refracted_ray, depth + 1, new_reflected_index);

		refracted_color *= oB->getTransmittance();
		color += refracted_color;
	}
	/**/
	return color;
}