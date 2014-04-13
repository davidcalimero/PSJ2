#include "Poly.h"

Poly::Poly(int nVertices, std::vector<glm::vec3> vertices, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture) : Object(fill_color, k_constants, transmittance, indexRefraction, texture){
	_nVertices = nVertices;
	_vertices = vertices;

	_normal = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));

	_box.pmin = vertices[0];
	_box.pmax = vertices[0];
	for (std::vector<glm::vec3>::iterator i = vertices.begin(); i != vertices.end(); i++){
		if (_box.pmin.x > (*i).x)
			_box.pmin.x = (*i).x;
		if (_box.pmin.y > (*i).y)
			_box.pmin.y = (*i).y;
		if (_box.pmin.z > (*i).z)
			_box.pmin.z = (*i).z;
		
		if (_box.pmax.x < (*i).x)
			_box.pmax.x = (*i).x;
		if (_box.pmax.y < (*i).y)
			_box.pmax.y = (*i).y;
		if (_box.pmax.z < (*i).z)
			_box.pmax.z = (*i).z;
	}

	//Mostra se o poligono foi bem criado (DEBUG)
	/** /
	std::cout << "Polygon created with " << _nVertices << " vertexes: " << std::endl;
	for (int i = 0; i < _nVertices; i++){
		std::cout << "(" << _vertices.at(i).x << ", " << _vertices.at(i).y << ", " << _vertices.at(i).z << ") " << std::endl;
	}
	/**/


}

bool Poly::rayInterception(Ray ray, glm::vec3 &point, glm::vec3 &normal){
	if (windingNumber(ray, point, normal)){
		normal = _normal;
		return true;
	}
	return false;
}

bool Poly::windingNumber(Ray ray, glm::vec3 &p, glm::vec3 &n){
	int wn = 0;

	//Calcular o ponto. se n intersectar, e' logo false, caso contrário calculamos o winding number
	glm::vec3 point;
	glm::vec3 normal;
	if (!returnPoint(ray, point, normal)) return false;

	// Calcular qual o eixo sobre qual fazer "cair" o poligono
	int axis = -1;
	if (_normal.x > _normal.y){
		if (_normal.x > _normal.z)
			axis = 0;
		else
			axis = 2;
	}
	else {
		if (_normal.y > _normal.z)
			axis = 1;
		else
			axis = 2;
	}

	glm::vec2 intPoint;
	if (axis == 0)
		intPoint = glm::vec2(point.y, point.z);
	if (axis == 1)
		intPoint = glm::vec2(point.z, point.x);
	if (axis == 2)
		intPoint = glm::vec2(point.x, point.y);

	int size = _vertices.size();
	for (int i = 0; i < size; i++){
		glm::vec2 nextVert, vert;
		if (axis == 0){
			vert = glm::vec2(_vertices[i].y, _vertices[i].z);
			nextVert = glm::vec2(_vertices[(i + 1) % size].y, _vertices[(i + 1) % size].z);
		}
		if (axis == 1){
			vert = glm::vec2(_vertices[i].z, _vertices[i].x);
			nextVert = glm::vec2(_vertices[(i + 1) % size].z, _vertices[(i + 1) % size].x);
		}
		if (axis == 2){
			vert = glm::vec2(_vertices[i].x, _vertices[i].y);
			nextVert = glm::vec2(_vertices[(i + 1) % size].x, _vertices[(i + 1) % size].y);
		}
		if (vert.y <= intPoint.y){
			if (nextVert.y > intPoint.y){
				if (isLeft(vert, nextVert, intPoint) > 0)
					++wn;
			}
		}
		else {
			if (nextVert.y <= intPoint.y){
				if (isLeft(vert, nextVert, intPoint) < 0)
					--wn;
			}
		}
	}

	return (wn != 0);
}

bool Poly::returnPoint(Ray ray, glm::vec3 &point, glm::vec3 &normal){

	normal = _normal;
	float D = -glm::dot(normal, _vertices[0]);

	float denominator = _normal.x * ray.D.x + _normal.y * ray.D.y + _normal.z * ray.D.z;
	//Se o denominador for igual a 0 o plano e o raio sao paralelos e como tal retorna false
	if (denominator == 0) return false;

	float ti = -(_normal.x * ray.O.x + _normal.y * ray.O.y + _normal.z * ray.O.z + D) / denominator;

	//Se a interseccao estiver por tras do raio original retorna false
	if (ti < 0) return false;

	//Distancia ao ponto de intercessao
	_t = ti;

	//Ponto onde existe interseccao
	point = glm::vec3(ray.O.x + ray.D.x * ti, ray.O.y + ray.D.y * ti, ray.O.z + ray.D.z * ti);

	return true;
}

float Poly::isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2){
	return ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y));
}

glm::vec3 Poly::getColorUV(glm::vec3 point, glm::vec3 normal){

	return _fill_color;
}