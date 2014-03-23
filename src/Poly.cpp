#include "Poly.h"

Poly::Poly(int nVertices, std::vector<glm::vec3> vertices, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction) : Object(fill_color, k_constants, transmittance, indexRefraction){
	_nVertices = nVertices;
	_vertices = vertices;

	//Mostra se o poligono foi bem criado (DEBUG)
	/** /
	std::cout << "Polygon created with " << _nVertices << " vertexes: " << std::endl;
	for (int i = 0; i < _nVertices; i++){
		std::cout << "(" << _vertices.at(i).x << ", " << _vertices.at(i).y << ", " << _vertices.at(i).z << ") " << std::endl;
	}
	/**/
}

bool Poly::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	// Insert code here :3
	/**/

	/**/
	
	return false;
}