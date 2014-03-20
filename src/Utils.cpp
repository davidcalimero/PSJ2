#include "Utils.h"

namespace Utils {

	void loadNFF(char* filename, glm::vec3 &from, glm::vec3 &at, glm::vec3 &up, float &angle, float &hither, glm::vec2 &resolution, std::vector<Light*> &lights, std::vector<Sphere*> &spheres, std::vector<Triangle*> &triangles){
		glm::vec3 bckg_color = glm::vec3(0, 0, 0);

		//DEBUG ISSUES
		/** /
		// Camera
		glm::vec3 from;
		glm::vec3 at;
		glm::vec3 up;
		float angle;
		float hither;
		glm::vec2 resolution;

		// Light
		std::vector<Light*> lights;

		// Spheres
		std::vector<Sphere*> spheres;
		
		// Triangles
		std::vector<Triangle*> triangles;
		/**/

		// Fill Color and shading variables
		Properties* properties;

		std::ifstream in(filename, std::ios::in);
		if (!in){
			std::cerr << "Cannot open " << filename << std::endl;
			exit(1);
		}

		std::string line;
		while (getline(in, line)) {
			if (line.substr(0, 2) == "b "){
				std::istringstream s(line.substr(2));
				s >> bckg_color.x >> bckg_color.y >> bckg_color.z;
				// DEBUG
				/** /
				std::cout << "background_color: (" << bckg_color.x << "," << bckg_color.y << "," << bckg_color.z << ")" << std::endl;
				/**/
			}
			else if (line.substr(0, 2) == "l "){
				Light* light = (Light*)malloc(sizeof(Light));
				std::istringstream s(line.substr(2));
				s >> light->position.x >> light->position.y >> light->position.z;
				// DEBUG
				/** /
				std::cout << "luz-posicao: (" << light->position.x << "," << light->position.y << "," << light->position.z << ")" << std::endl;
				/**/
				// color -- incomplete (pode n aparecer)
				s >> light->color.x >> light->color.y >> light->color.z;
				// DEBUG
				/** /
				std::cout << "luz-cor: (" << light->color.x << "," << light->color.y << "," << light->color.z << ")" << std::endl;
				/**/
				lights.push_back(light);
			}
			else if (line.substr(0, 2) == "s "){
				Sphere* sphere = (Sphere*)malloc(sizeof(Sphere));
				sphere->properties = *properties;
				std::istringstream s(line.substr(2));
				s >> sphere->center.x >> sphere->center.y >> sphere->center.z >> sphere->radius;
				spheres.push_back(sphere);
				// DEBUG
				/** /
				std::cout << "sphere: center in (" << sphere->center.x << "," << sphere->center.y << "," << sphere->center.z << ") and with radius " << sphere->radius << std::endl;
				/**/
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
				s 	>> properties->fill_color.x >> properties->fill_color.y >> properties->fill_color.z 
					>> properties->k_constants.x >> properties->k_constants.y >> properties->k_constants.z 
					>> properties->transmittance >> properties->indexRefraction;
				// DEBUG
				/** /
				std::cout 	<< " Mudanca de Properties:" << std::endl
					<< "Color (" << properties->fill_color.x << "," << properties->fill_color.y << "," << properties->fill_color.z << ") " << std::endl
					<< "Kd " << properties->k_constants.x << ", Ks" << properties->k_constants.y << ", shininess " << properties->k_constants.z << ") " << std::endl
					<< "transmittance " << properties->transmittance << " and index of refraction " << properties->indexRefraction << std::endl;
				/**/
			}
			else if (line.substr(0, 3) == "pl ") {
				Triangle* triangle = (Triangle*)malloc(sizeof(Triangle));
				triangle->properties = *properties;
				std::istringstream s(line.substr(3));
				glm::vec3 p1, p2, p3;
				s >> triangle->p1.x >> triangle->p1.y >> triangle->p1.z
					>> triangle->p2.x >> triangle->p2.y >> triangle->p2.z
					>> triangle->p3.x >> triangle->p3.y >> triangle->p3.z;
				// DEBUG
				/** /
				std::cout 	<< "Triangle:" << std::endl
					<< "P1 (" << triangle->p1.x << "," << triangle->p1.y << "," << triangle->p1.z << ") " << std::endl
					<< "P2 (" << triangle->p2.x << "," << triangle->p2.y << "," << triangle->p2.z << ") " << std::endl
					<< "P3 (" << triangle->p3.x << "," << triangle->p3.y << "," << triangle->p3.z << ") " << std::endl;
				/**/
				triangles.push_back(triangle);
			}
		}
		// DEBUG
		/** /
		std::cout 	<< "viewport:" << std::endl 
					<< "from (" << from.x << "," << from.y << "," << from.z << ") " << std::endl 
					<< "at (" << at.x << "," << at.y << "," << at.z << ") " << std::endl
					<< "up (" << up.x << "," << up.y << "," << up.z << ") " << std::endl
					<< "with angle " << angle << std::endl
					<< "hither " << hither << std::endl
					<< "and resolution (" << resolution.x << "," << resolution.y << ") " << std::endl;
		/** /
		for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it){
			std::cout << "sphere: center in (" << (*it)->center.x << "," << (*it)->center.y << "," << (*it)->center.z << ") and with radius " << (*it)->radius << std::endl;
			std::cout << "Sphere:" << std::endl
				<< "Properties"  << std::endl
				<< "Color (" << (*it)->properties.fill_color.x << "," << (*it)->properties.fill_color.y << "," << (*it)->properties.fill_color.z << ") " << std::endl
				<< "Kd " << (*it)->properties.k_constants.x << ", Ks " << (*it)->properties.k_constants.y << ", shininess " << (*it)->properties.k_constants.z << ") " << std::endl
				<< "transmittance " << (*it)->properties.transmittance << " and index of refraction " << (*it)->properties.indexRefraction << std::endl;
		}
		/**/
	}
}
