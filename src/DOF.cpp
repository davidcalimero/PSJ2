#include "DOF.h"

glm::vec3 DOF::DepthOfField(glm::vec2 pixel, float focalLength){
	glm::vec3 color;
	glm::vec3 colors = glm::vec3(0);

	Ray ray = Scene::getInstance().GetCamera()->PrimaryRay(pixel.x, pixel.y);

	//pointAimed is the position of pixel on focal plane 
	glm::vec3 P = ray.O + (focalLength * ray.D);

	//radius of sphere of the lens of DOF
	for (int di = 0; di < AREA_SQUARE_DOF * AREA_SQUARE_DOF; di++){ // shooting random rays

		float x = (di % AREA_SQUARE_DOF - (AREA_SQUARE_DOF * 0.5f)) / AREA_SQUARE_DOF;
		float y = ((float)floor(di / AREA_SQUARE_DOF) - (AREA_SQUARE_DOF * 0.5f)) / AREA_SQUARE_DOF;

		//std::cout << "x = " << x << " and y = " << y << std::endl;

		float du = (rand() / float(RAND_MAX + 1)) / AREA_SQUARE_DOF; //generating random number
		float dn = (rand() / float(RAND_MAX + 1)) / AREA_SQUARE_DOF;

		glm::vec3 u = Scene::getInstance().GetCamera()->GetU();
		glm::vec3 n = Scene::getInstance().GetCamera()->GetN();

		//std::cout << "du = " << du << " and dn = " << dn << std::endl;

		Ray newRay;

		float newU = y + du;
		float newN = x + dn;

		//std::cout << "newU = " << newU << " and newN = " << newN << std::endl;

		// creating new camera position(or ray start using jittering)
		newRay.O = ray.O + newU*u + newN*n;

		//getting the new direction of ray
		newRay.D = P - newRay.O;

		newRay.D = glm::normalize(newRay.D);

		color = RayTracing::rayTracing(newRay, 1, 1);
		colors += color;
	}
	color.r = colors.r / (AREA_SQUARE_DOF * AREA_SQUARE_DOF);
	color.g = colors.g / (AREA_SQUARE_DOF * AREA_SQUARE_DOF);
	color.b = colors.b / (AREA_SQUARE_DOF * AREA_SQUARE_DOF);

	return color;
}
