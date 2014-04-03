#include "Scene.h"

Scene::Scene(){}
Scene::~Scene(){}

//Retorna a instancia singleton de scene
Scene& Scene::getInstance(){
	static Scene  instance;
	return instance;
}

glm::vec3 Scene::GetBckgColor(){
	return _bckg_color;
}

Camera* Scene::GetCamera(){
	return _camera;
}

bool Scene::loadNFF(char* filename){
	glm::vec3 from;
	glm::vec3 at;
	glm::vec3 up;
	float angle;
	float hither;
	glm::vec2 resolution;

	bool result = loadNFF(filename, from, at, up, angle, hither, resolution, _bckg_color, _lights, _objects);

	if (!result) return result;
	_camera = new Camera(from, at, up, angle, 0, 0, (int)resolution.x, (int)resolution.y);

	return true;
}

std::vector<Object*> Scene::GetObjects(){
	return _objects;
}

std::vector<Light*> Scene::GetLights(){
	return _lights;
}

void Scene::printPropertiesValues(Properties* properties){
	std::cout 	<< "Mudanca de Properties:" << std::endl
	<< "Color (" << properties->fill_color.x << "," << properties->fill_color.y << "," << properties->fill_color.z << ") " << std::endl
	<< "Kd " << properties->k_constants.x << ", Ks " << properties->k_constants.y << ", shininess " << properties->k_constants.z << std::endl
	<< "transmittance " << properties->transmittance << " and index of refraction " << properties->indexRefraction << std::endl;
}

void Scene::showFinalValues(){
	// Show Background Color
	std::cout << "background Color: (" << _bckg_color.x << "," << _bckg_color.y << "," << _bckg_color.z << ")" << std::endl;

	// Show Light values
	int i = 1;
	for (std::vector<Light*>::iterator it = _lights.begin(); it != _lights.end(); it++){
		std::cout << "light nr " << i;
		std::cout << " with position: (" << (*it)->position.x << "," << (*it)->position.y << "," << (*it)->position.z << ")";
		std::cout << " and color: (" << (*it)->color.x << "," << (*it)->color.y << "," << (*it)->color.z << ")" << std::endl;
		i++;
	}
}	

bool Scene::loadNFF(char* filename, glm::vec3 &from, glm::vec3 &at, glm::vec3 &up, float &angle, float &hither, glm::vec2 &resolution, glm::vec3 &bckg_color, std::vector<Light*> &lights, std::vector<Object*> &objects){

	// Fill Color and shading variables
	Properties* properties;

	// Load file
	std::ifstream in(filename, std::ios::in);
	if (!in){
		std::cerr << "Cannot open " << filename << std::endl;
		return false;
	}

	// Parse the file
	std::string line;
	while (getline(in, line)) {
		if (line.substr(0, 2) == "b "){
			std::istringstream s(line.substr(2));
			s >> bckg_color.x >> bckg_color.y >> bckg_color.z;
		}
		else if (line.substr(0, 2) == "l "){
			Light* light = (Light*)malloc(sizeof(Light));
			std::istringstream s(line.substr(2));
			s >> light->position.x >> light->position.y >> light->position.z;
			s >> light->color.x >> light->color.y >> light->color.z;
			lights.push_back(light);
		}
		else if (line.substr(0, 2) == "s "){
			glm::vec3 center;
			float radius;

			std::istringstream s(line.substr(2));
			s >> center.x >> center.y >> center.z >> radius;
			Sphere* sphere = new Sphere(center, radius, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction);
			objects.push_back(sphere);
		}
		else if (line.substr(0, 5) == "from ") {
			std::istringstream s(line.substr(5));
			s >> from.x >> from.y >> from.z;
		}
		else if (line.substr(0, 3) == "at ") {
			std::istringstream s(line.substr(3));
			s >> at.x >> at.y >> at.z;
		}
		else if (line.substr(0, 3) == "up ") {
			std::istringstream s(line.substr(3));
			s >> up.x >> up.y >> up.z;
		}
		else if (line.substr(0, 6) == "angle ") {
			std::istringstream s(line.substr(6));
			s >> angle;
		}
		else if (line.substr(0, 7) == "hither ") {
			std::istringstream s(line.substr(7));
			s >> hither;
		}
		else if (line.substr(0, 11) == "resolution ") {
			std::istringstream s(line.substr(11));
			s >> resolution.x >> resolution.y;
		}
		else if (line.substr(0, 2) == "f ") {
			properties = (Properties*)malloc(sizeof(Properties));
			std::istringstream s(line.substr(2));
			s	>> properties->fill_color.x >> properties->fill_color.y >> properties->fill_color.z
				>> properties->k_constants.x >> properties->k_constants.y >> properties->k_constants.z
				>> properties->transmittance >> properties->indexRefraction;
			//printPropertiesValues(properties);
		}
		else if (line.substr(0, 3) == "pl ") {

			glm::vec3 p1, p2, p3;
			std::istringstream s(line.substr(3));
			s >> p1.x >> p1.y >> p1.z
				>> p2.x >> p2.y >> p2.z
				>> p3.x >> p3.y >> p3.z;
			Plane* plane = new Plane(p1, p2, p3, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction);
			objects.push_back(plane);
		}
		else if (line.substr(0, 2) == "p ") {
			int nVertices;
			std::istringstream s(line.substr(2));
			s >> nVertices;
			glm::vec3 t(0.0);
			std::vector<glm::vec3> vertices(nVertices, t);
			for (int i = 0; i < nVertices; i++){
				getline(in, line);
				std::istringstream vertice(line.substr(0));
				vertice >> vertices.at(i).x >> vertices.at(i).y >> vertices.at(i).z;
			}
			if (nVertices == 3){
				Triangle* triforce = new Triangle(vertices.at(0), vertices.at(1), vertices.at(2), properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction);
				objects.push_back(triforce);

			}else{
				Poly* poly = new Poly(nVertices, vertices, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction);
				objects.push_back(poly);
			}
		}
	}

	//showFinalValues();
	return true;
}

