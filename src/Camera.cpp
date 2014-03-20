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

	//Mais inicialização....
	_d = (_eye - _at).length();
	_h = 2 * _d * tan(_fovy / 2);
	_w = _ResX / _ResY * _h;

	_ze = (_eye - _at) / (float)(_eye - _at).length();
	_xe = glm::cross(_up, _ze) / (float)(glm::cross(_up, _ze)).length();
	_ye = glm::cross(_ze, _xe);

}

Ray Camera::PrimaryRay(int x, int y){
	Ray r;
	r.O = _eye;
	r.D = -_d * _ze + _h *(y / (float)(_ResY - 1) - 0.5f) * _ye + _w *(x / (float)(_ResX - 1) - 0.5f) *_xe;
	r.D = glm::normalize(r.D);
	//std::cout << r.O.x << " " << r.O.y << " " << r.O.z << " <> " << r.D.x << " " << r.D.y << " " << r.D.z << " " << std::endl;
	return r;
}

Camera::~Camera(){

}

int Camera::GetResX(){
	return _ResX;
}

int Camera::GetResY(){
	return _ResY;
}