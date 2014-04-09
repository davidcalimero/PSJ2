#include "DOF.h"

glm::vec3 DOF::DepthOfField(glm::vec2 pixel, float focalLenght){
	glm::vec3 color;
	glm::vec3 colors = glm::vec3(0);

	Ray ray = Scene::getInstance().GetCamera()->PrimaryRay(pixel.x, pixel.y);

	//pointAimed is the position of pixel on focal plane 
	glm::vec3 P = ray.O + focalLenght * ray.D;

	//radius of sphere of the lens of DOF
	float r = 1;
	for (int di = 0; di < N_DOF_RAYS; di++){ // shooting random rays
		float du = rand() / float(RAND_MAX + 1); //generating random number
		float dv = rand() / float(RAND_MAX + 1);

		glm::vec3 u = Scene::getInstance().GetCamera()->GetU();
		glm::vec3 v = Scene::getInstance().GetCamera()->GetV();

		Ray newRay;

		// creating new camera position(or ray start using jittering)
		newRay.O = ray.O - (r / 2)*u - (r / 2)*v + r*(du)*u + r*(dv)*v;

		//getting the new direction of ray
		newRay.D = P - newRay.O;

		newRay.D = glm::normalize(newRay.D);

		color = RayTracing::rayTracing(newRay, 1, 1);
		colors += color;
	}
	color.r = colors.r / N_DOF_RAYS;
	color.g = colors.g / N_DOF_RAYS;
	color.b = colors.b / N_DOF_RAYS;

	return color;
}
