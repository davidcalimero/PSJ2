#include "Scene.h"

Scene::Scene(){
	_grid = false;
}
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

	// Regular Grid Creation
	_grid = new RegularGrid(_objects);

	return true;
}

std::vector<Object*> Scene::GetObjects(Ray ray){
	if (_usingGrid)
		return _grid->traversalAlgorithm(ray);

	return _objects;
}

std::vector<Light*> Scene::GetLights(){
	return _lights;
}

void Scene::printPropertiesValues(Properties* properties){
	std::cout << "Mudanca de Properties:" << std::endl
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
			Sphere* sphere = new Sphere(center, radius, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
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
			std::string textureFile;
			properties = (Properties*)malloc(sizeof(Properties));
			std::istringstream s(line.substr(2));
			s >> properties->fill_color.x >> properties->fill_color.y >> properties->fill_color.z
				>> properties->k_constants.x >> properties->k_constants.y >> properties->k_constants.z
				>> properties->transmittance >> properties->indexRefraction >> textureFile;
			if (textureFile != ""){
				properties->texture = Utils::loadTexture(textureFile.c_str());
			}
			else{
				properties->texture = NULL;
			}

			//printPropertiesValues(properties);
		}
		else if (line.substr(0, 3) == "pl ") {

			glm::vec3 p1, p2, p3;
			std::istringstream s(line.substr(3));
			s >> p1.x >> p1.y >> p1.z
				>> p2.x >> p2.y >> p2.z
				>> p3.x >> p3.y >> p3.z;
			Plane* plane = new Plane(p1, p2, p3, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
			objects.push_back(plane);
		}
		else if (line.substr(0, 4) == "cyl ") {

			glm::vec3 p1, p2;
			float raio;
			std::istringstream s(line.substr(3));
			s >> p1.x >> p1.y >> p1.z
				>> p2.x >> p2.y >> p2.z
				>> raio;
			Cylinder* cylinder = new Cylinder(p1, p2, raio, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
			objects.push_back(cylinder);
		}
		else if (line.substr(0, 4) == "ply ") {
			glm::vec3 pos;
			std::string filename;
			std::istringstream s(line.substr(3));
			s >> pos.x >> pos.y >> pos.z >> filename;
			loadPLY(filename.c_str(), pos, objects, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
		}

		else if (line.substr(0, 3) == "bb ") {

			glm::vec3 p1, p2;
			std::istringstream s(line.substr(3));
			s >> p1.x >> p1.y >> p1.z
				>> p2.x >> p2.y >> p2.z;
			AABB* aabb = new AABB(p1, p2, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
			objects.push_back(aabb);
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
				Triangle* triforce = new Triangle(vertices, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
				objects.push_back(triforce);

			}
			else{
				Poly* poly = new Poly(nVertices, vertices, properties->fill_color, properties->k_constants, properties->transmittance, properties->indexRefraction, properties->texture);
				objects.push_back(poly);
			}
		}
	}

	//showFinalValues();
	return true;
}

void Scene::toggleGrid(){
	_usingGrid = !_usingGrid;
	std::cout << "Grid: " << (_usingGrid ? "ON" : "OFF") << std::endl;
}

void Scene::loadPLY(const char *objFile, glm::vec3 pos, std::vector<Object*> &objects, glm::vec3 fill_color, glm::vec3 k_constants, float transmittance, float indexRefraction, tImageTGA *texture){
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;


	PlyFile *input = read_ply(fopen(objFile, "r"));
	//Vertex ***vertices;
	//Face *** faces;
	unsigned int facecount = 0;
	unsigned int vertexcount = 0;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec2> uvs;

	int i, j;

	// go through the element types
	for (i = 0; i < input->num_elem_types; i = i + 1) {
		int count;

		// setup the element for reading and get the element count
		char* element = setup_element_read_ply(input, i, &count);

		// get vertices
		if (strcmp("vertex", element) == 0) {
			//*vertices = (Vertex**)malloc(sizeof(Vertex)* count);
			vertexcount = count;

			// run through the properties and store them
			for (j = 0; j < input->elems[i]->nprops; j = j + 1) {
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				if (strcmp("x", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "x";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, XYZ[0]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if (strcmp("y", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "y";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, XYZ[1]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if (strcmp("z", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "z";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, XYZ[2]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if (strcmp("nx", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "nx";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, N[0]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*vertexnormals = 1;
				}
				else if (strcmp("ny", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "ny";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, N[1]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*vertexnormals = 1;
				}
				else if (strcmp("nz", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "nz";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, N[2]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*vertexnormals = 1;
				}
				else if (strcmp("s", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "s";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, UV[0]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);

				}
				else if (strcmp("t", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "t";
					setup.internal_type = Float32;
					setup.offset = offsetof(Vertex, UV[1]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);

				}
				// dunno what it is
				else {
					//std::cerr << "unknown property type found in " << element << ": " << property->name << std::endl;
				}
			}

			// copy the data
			for (j = 0; j < count; j = j + 1) {
				Vertex* v = (Vertex*)malloc(sizeof(Vertex));
				get_element_ply(input, (void*)(v));

				verts.push_back(glm::vec3(v->XYZ[0], v->XYZ[1], v->XYZ[2]));
				norms.push_back(glm::vec3(v->N[0], v->N[1], v->N[2]));
				uvs.push_back(glm::vec2(v->UV[0], v->UV[1]));
			}
		}
		// get faces
		else if (strcmp("face", element) == 0) {
			facecount = count;

			// run through the properties and store them
			for (j = 0; j < input->elems[i]->nprops; j = j + 1) {
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				if (strcmp("vertex_indices", property->name) == 0 &&
					property->is_list == PLY_LIST) {

					setup.name = "vertex_indices";
					setup.internal_type = Uint32;
					setup.offset = offsetof(Face, vertices);
					setup.count_internal = Uint32;
					setup.count_offset = offsetof(Face, count);

					setup_property_ply(input, &setup);
				}
				else if (strcmp("nx", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "nx";
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, N[0]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*facenormals = 1;
				}
				else if (strcmp("ny", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "ny";
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, N[1]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*facenormals = 1;
				}
				else if (strcmp("nz", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

					setup.name = "nz";
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, N[2]);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					//*facenormals = 1;
				}
				// dunno what it is
				else {
					//std::cerr << "unknown property type found in " << element << ": " << property->name << std::endl;
				}
			}

			// copy the data
			for (j = 0; j < count; j = j + 1) {
				//(*faces)[j] = (Face*)malloc(sizeof(Face));
				Face *f = (Face*)malloc(sizeof(Face));
				get_element_ply(input, (void*)(f));

				unsigned int v1 = f->vertices[0];
				unsigned int v2 = f->vertices[1];
				unsigned int v3 = f->vertices[2];

				//std::cout << f->vertices[0] << " " << f->vertices[1] << " " << f->vertices[2] << " " << std::endl;

				std::vector<glm::vec3> triangle_verts;
				std::vector<glm::vec3> triangle_normals;
				std::vector<glm::vec2> triangle_uvs;

				//out_vertices.push_back(verts[v1]);
				//out_uvs.push_back(uvs[v1]);
				//out_normals.push_back(norms[v1]);

				triangle_verts.push_back(verts[v1] + pos);
				triangle_normals.push_back(norms[v1]);
				triangle_uvs.push_back(uvs[v1]);


				//out_vertices.push_back(verts[v2]);
				//out_uvs.push_back(uvs[v2]);
				//out_normals.push_back(norms[v2]);

				triangle_verts.push_back(verts[v2] + pos);
				triangle_normals.push_back(norms[v2]);
				triangle_uvs.push_back(uvs[v2]);

				//out_vertices.push_back(verts[v3]);
				//out_uvs.push_back(uvs[v3]);
				//out_normals.push_back(norms[v3]);

				triangle_verts.push_back(verts[v3] + pos);
				triangle_normals.push_back(norms[v3]);
				triangle_uvs.push_back(uvs[v3]);

				PlyTriangle* plyTriangle = new PlyTriangle(triangle_verts, triangle_normals, triangle_uvs, fill_color, k_constants, transmittance, indexRefraction, texture);
				objects.push_back(plyTriangle);
			}
		}
		// who knows?
		else {
			//std::cerr << "unknown element type found: " << element << std::endl;
		}
	}
}

bool Scene::isUsingGrid(){
	return _usingGrid;
}