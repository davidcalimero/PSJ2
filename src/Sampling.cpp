#include "Sampling.h"

glm::vec3 Sampling::recursiveFill(glm::vec2 subpixel, int recursive, std::vector<std::vector<glm::vec3>> &buffer){

	//							ponto 3	 ______________  ponto 4
	//									|			   |
	//									|	   CP	   |
	//							ponto 1	|______________| ponto 2

	Ray ray;

	// Encontrar as cores nos cantos do pixel em centralPoint
	glm::vec2 point1 = glm::vec2(subpixel.x - (1 / pow(2, recursive)), subpixel.y - (1 / pow(2, recursive)));
	glm::vec2 point2 = glm::vec2(subpixel.x + (1 / pow(2, recursive)), subpixel.y - (1 / pow(2, recursive)));
	glm::vec2 point3 = glm::vec2(subpixel.x - (1 / pow(2, recursive)), subpixel.y + (1 / pow(2, recursive)));
	glm::vec2 point4 = glm::vec2(subpixel.x + (1 / pow(2, recursive)), subpixel.y + (1 / pow(2, recursive)));

	//Calcular a cor dos vários pontos
	ray = Scene::getInstance().GetCamera()->PrimaryRay(point1.x, point1.y);
	glm::vec3 colorP1 = RayTracing::rayTracing(ray, 1, 1);

	ray = Scene::getInstance().GetCamera()->PrimaryRay(point2.x, point2.y);
	glm::vec3 colorP2 = RayTracing::rayTracing(ray, 1, 1);

	ray = Scene::getInstance().GetCamera()->PrimaryRay(point3.x, point3.y);
	glm::vec3 colorP3 = RayTracing::rayTracing(ray, 1, 1);

	ray = Scene::getInstance().GetCamera()->PrimaryRay(point4.x, point4.y);
	glm::vec3 colorP4 = RayTracing::rayTracing(ray, 1, 1);

	if (recursive < MAX_SAMPLING){
		//Super Sampling
		float diff12 = abs(colorP1.r - colorP2.r) + abs(colorP1.g - colorP2.g) + abs(colorP1.b - colorP2.b);
		float diff13 = abs(colorP1.r - colorP3.r) + abs(colorP1.g - colorP3.g) + abs(colorP1.b - colorP3.b);
		float diff24 = abs(colorP4.r - colorP2.r) + abs(colorP4.g - colorP2.g) + abs(colorP4.b - colorP2.b);
		float diff34 = abs(colorP4.r - colorP3.r) + abs(colorP4.g - colorP3.g) + abs(colorP4.b - colorP3.b);

		if (diff12 > COLOR_THRESHOLD || diff13 > COLOR_THRESHOLD || diff24 > COLOR_THRESHOLD || diff34 > COLOR_THRESHOLD){
			glm::vec2 p1 = (subpixel + point1) * 0.5f;
			colorP1 = recursiveFill(p1, recursive + 1, buffer);

			glm::vec2 p2 = (subpixel + point2) * 0.5f;
			colorP2 = recursiveFill(p2, recursive + 1, buffer);

			glm::vec2 p3 = (subpixel + point3) * 0.5f;
			colorP3 = recursiveFill(p3, recursive + 1, buffer);

			glm::vec2 p4 = (subpixel + point4) * 0.5f;
			colorP4 = recursiveFill(p4, recursive + 1, buffer);
		}
	}

	//Cor Final
	return (colorP1 + colorP2 + colorP3 + colorP4) / 4.0f;
}