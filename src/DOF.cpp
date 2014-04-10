#include "DOF.h"

glm::vec3 DOF::DepthOfField(glm::vec2 pixel){
	glm::vec3 color;
	glm::vec3 colors = glm::vec3(0);


	Ray ray = Scene::getInstance().GetCamera()->PrimaryRay(pixel.x, pixel.y);

	if (!DOF_ACTIVE)
		return RayTracing::rayTracing(ray, 1, 1);

	//pointAimed is the position of pixel on focal plane 
	glm::vec3 P = ray.O + (FOCALLENGTH * ray.D);

	float r = 0.5;  //raio da esfera (tem de ser unitario)
	for (int di = 0; di < N_DEPTH_RAYS; di++){

		float du = rand() / float(RAND_MAX + 1);
		float dv = rand() / float(RAND_MAX + 1);

		Ray newRay;
		glm::vec3 u = Scene::getInstance().GetCamera()->GetU();
		glm::vec3 v = Scene::getInstance().GetCamera()->GetV();

		// creating new camera position(or ray start using jittering)
		newRay.O = ray.O - (r / 2.0f)*u - (r / 2.0f)*v + r*(du)*u + r*(dv)*v;
		//getting the new direction of ray
		newRay.D = P - newRay.O;
		newRay.D = glm::normalize(newRay.D);

		colors += RayTracing::rayTracing(newRay, 1, 1);
	}

	//Final Color
	color = colors / (float) N_DEPTH_RAYS;

	return color;
}
