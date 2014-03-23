#include "Camera.h"


Camera::Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float fovy, float near, float far, int ResX, int ResY){
	//Inicializar variaveis
	_eye = eye;
	_at = at;
	_up = up;
	_fovy = fovy;
	_near = near;
	_far = far;
	_ResX = ResX;
	_ResY = ResY;

	//Inicializacao de variaveis utilizadas para calcular o raio principal
	_d = Utils::norma(_eye - _at);
	_h = (float) 2 * _d * tan(_fovy * PI/180.0f / 2.0f);
	_w = (float)_ResX / _ResY * _h;

	_ze = (_eye - _at) / _d;
	_xe = glm::cross(_up, _ze) / Utils::norma(glm::cross(_up, _ze));
	_ye = glm::cross(_ze, _xe);


	// Show Camera values (DEBUG)
	/** /
	std::cout 	<< "viewport:" << std::endl
	<< "from (" << from.x << "," << from.y << "," << from.z << ") " << std::endl
	<< "at (" << at.x << "," << at.y << "," << at.z << ") " << std::endl
	<< "up (" << up.x << "," << up.y << "," << up.z << ") " << std::endl
	<< "with angle " << fovy << std::endl
	<< "hither " << near << std::endl
	<< "and resolution (" << ResX << "," << ResY << ") " << std::endl;
	/**/
}


//Cria um raio que será lancado da posicao xy em coordenadas 
Ray Camera::PrimaryRay(int x, int y){
	Ray r;
	//Origem do raio
	r.O = _eye;
	//Direccao do raio
	r.D = -_d * _ze + _h *(y / (float)(_ResY - 1) - 0.5f) * _ye + _w *(x / (float)(_ResX - 1) - 0.5f) *_xe;
	r.D = glm::normalize(r.D);
	return r;
}

Camera::~Camera(){

}

//Retorna a largura da janela
int Camera::GetResX(){
	return _ResX;
}

//Retorna a altura da janela
int Camera::GetResY(){
	return _ResY;
}

//Retorna a posicao da camara (eye)
glm::vec3 Camera::GetPos(){
	return _eye;
}