#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <glm.hpp>
#include <vector>
#include <thread>

#include "Scene.h"

#define MAX_DEPTH 6
#define N_THREADS 8

Scene* scene;
int RES_X, RES_Y; //resolucao do ecra
std::vector<std::vector<glm::vec3>> buffer; //buffer onde as threads vao pintar
std::vector<std::thread> threads; //threads que vao ser utilizadas
glm::vec3 at(1.0f, 0.0f, 0.0f); //Valores de atenuacao

glm::vec3 rayTracing(Ray ray, int depth, float ior);


//Verifica se um raio em direcao a cada fonte de luz tem algum objecto pelo meio
bool isAffectedByLight(Ray ray){
	std::vector<Object*> objs = scene->GetObjects();
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 point;
		glm::vec3 normal;
		//Se existir uma intercessao esse ponto nao e afectado directamente pela luz
		if (((Object*)(*it))->rayInterception(ray, point, normal))
			return false;
	}
	return true;
}


//Calcular Raios Sombra
glm::vec3 shade(Object * oB, glm::vec3 normal, glm::vec3 point){
	glm::vec3 color;

	std::vector<Light*> lights = scene->GetLights();
	if (lights.size() > 0) color = glm::vec3(0);
	for (std::vector<Light*>::iterator il = lights.begin(); il != lights.end(); il++){
		glm::vec3 L = glm::normalize((*il)->position - point);
		glm::vec3 H = glm::normalize(L + glm::normalize(scene->GetCamera()->GetPos() - point));
		float lightDist = glm::distance((*il)->position, point);
		
			if (glm::dot(L, normal) > 0 && glm::dot(H, normal) > 0){
			Ray shadow;
			//Margem de erro para o caso do raio trespassar a esfera 
			shadow.O = point + 0.001f*L;
			shadow.D = L;
			//Se nao existir um objecto em direcao a luz, calcular a cor do ponto com a respectiva luz
			if (isAffectedByLight(shadow)){
				float attenuation = 1.0f / (at.x + at.y * lightDist + at.z * lightDist * lightDist);
				color += ((oB->Get_k_constants().x * glm::dot(L, normal)) +
					(oB->Get_k_constants().y * pow(glm::dot(H, normal), oB->Get_k_constants().z) * attenuation)) * (*il)->color * oB->GetFillColor();
			}
		}
	}

	return color;
}


//Encontra o objecto mais proximo da origem do raio
Object * nearestIntersection(Ray ray, glm::vec3 &normal, glm::vec3 &point){
	std::vector<Object*> objs = scene->GetObjects();
	float nearestPoint = -1;
	Object * object = NULL;

	//Determinar interseção mais proxima com um objecto
	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		glm::vec3 nPoint;
		glm::vec3 nNormal;
		bool intercept = ((Object*)(*it))->rayInterception(ray, nPoint, nNormal);
		float dist = glm::distance(ray.O, nPoint);

		//Se estiver mais proximo actulizar valores
		if ((nearestPoint == -1 || dist < nearestPoint) && intercept){
			nearestPoint = dist;
			point = nPoint;
			normal = nNormal;
			object = *it;
		}
	}
	return object;
}


//Calcular Raios de Reflexao
void calculateReflection(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color){
	if (oB->Get_k_constants().y != 0){
		glm::vec3 reflected_color;
		glm::vec3 R = ray.D - 2 * glm::dot(ray.D, normal) * normal; //Raio de reflexao
		Ray reflected_ray;
		reflected_ray.O = point + 0.001f*R;
		reflected_ray.D = R;
		reflected_color = rayTracing(reflected_ray, depth + 1, ior);
		color += reflected_color * oB->Get_k_constants().y;
	}
}


//Calcular Raios de Refraccao
void calculateRefraction(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color){
	if (oB->getTransmittance() != 0){
		glm::vec3 refracted_color;

		// Ver questão do sinal do ray.D	
		glm::vec3 vt = glm::dot(-ray.D, normal) * normal + ray.D;
		float sin_teta_i = Utils::norma(vt);

		// Ver se está dentro ou fora do objecto
		float sin_teta_t;
		float new_reflected_index;

		if (ior != 1) //de dentro para fora
			new_reflected_index = 1;
		else //de fora para dentro
			new_reflected_index = oB->getRefractionIndex();

		//Lei snell
		sin_teta_t = ior / new_reflected_index * sin_teta_i;

		//Se se verificar não houve reflexao perfeita
		if (sin_teta_t*sin_teta_t <= 1){
			float cos_teta_t = sqrt(1 - (sin_teta_t * sin_teta_t));
			glm::vec3 t = glm::normalize(vt);
			glm::vec3 rt = sin_teta_t*t + cos_teta_t * (-normal); //Raio de refraccao
			rt = glm::normalize(rt);
			Ray refracted_ray;
			refracted_ray.O = point + 0.001f*rt;
			refracted_ray.D = rt;
			refracted_color = rayTracing(refracted_ray, depth + 1, new_reflected_index);
			color += refracted_color * oB->getTransmittance();
		}
	}
}


//Ray Tracing
glm::vec3 rayTracing(Ray ray, int depth, float ior){
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 color(0);

	//Objecto mais proximo
	Object * oB = nearestIntersection(ray, normal, point);

	//Se nao existir uma intercepcao deve ser dada a cor do background
	if (oB == NULL)
		return scene->GetBckgColor();

	//Se ainda nao atingimos o limite temos de calcular os raios secundarios
	if (depth < MAX_DEPTH){
		
		//Refleccao
		glm::vec3 r1Color;
		//std::thread t1(calculateReflection, ray, oB, point, normal, depth, ior, r1Color);
		calculateReflection(ray, oB, point, normal, depth, ior, r1Color);

		//Refraccao
		glm::vec3 r2Color;
		//std::thread t2(calculateRefraction, ray, oB, point, normal, depth, ior, r2Color);
		calculateRefraction(ray, oB, point, normal, depth, ior, r2Color);

		//t1.join();
		//t2.join();

		color += r1Color + r2Color;
	}

	//Cor final
	return color + shade(oB, normal, point);
}


//Enviar um raio
void sendRay(int xi, int yi, int w, int h) {
	for (int y = yi; y < yi + h; y++) {
		for (int x = xi; x < xi + w; x++) {
			Ray ray = scene->GetCamera()->PrimaryRay(x, y);
			buffer[y][x] = rayTracing(ray, 1, 1);
		}
	}
}


//Cria N_THREADS que vao utilizar a funcao sendRay e faz join a cada uma delas
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

	//As threads vao actualizar o buffer com as respectivas cores
	createThreadsAndJoin();

	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//Pinta o pixel
			glBegin(GL_POINTS);
			glColor3f(buffer[y][x].x, buffer[y][x].y, buffer[y][x].z);
			glVertex2f((GLfloat)x, (GLfloat)y);
			glEnd();
		}
	}

	glFlush();
	std::cout << "Terminou!" << std::endl;
}


//MAIN
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
	glutCreateWindow("FRED Ray Tracing");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}