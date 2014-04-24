#include "RayTracing.h"

//Verifica se um raio em direcao a cada fonte de luz tem algum objecto pelo meio
bool RayTracing::isAffectedByLight(Ray ray){
	std::vector<Object*> objs = Scene::getInstance().GetObjects(ray);

	//Caso usar grid e haver objectos intersectados
	if (Scene::getInstance().isUsingGrid() && objs.size() > 0)
		return false;

	for (std::vector<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
		//Se existir uma intercessao esse ponto nao e afectado directamente pela luz
		float t;
		 if (((Object*)(*it))->rayInterception(ray, glm::vec3(), glm::vec3(), t))
			return false;
	}
	return true;
}


//Calcula a sombra no point dada uma determinada luz
glm::vec3 RayTracing::calculateShadow(glm::vec3 lighPosition, glm::vec3 lighColor, glm::vec3 point, glm::vec3 normal, Object *oB){
	glm::vec3 at(1.0f, 0.0f, 0.0f); //Valores de atenuacao
	glm::vec3 color(0);
	glm::vec3 L = glm::normalize(lighPosition - point);
	glm::vec3 H = glm::normalize(L + glm::normalize(Scene::getInstance().GetCamera()->GetPos() - point));
	float lightDist = glm::distance(lighPosition, point);

	if (glm::dot(L, normal) > 0 && glm::dot(H, normal) > 0){
		Ray shadow;
		//Margem de erro para o caso do raio trespassar a esfera 
		shadow.O = point + 0.001f*L;
		shadow.D = L;
		//Se nao existir um objecto em direcao a luz, calcular a cor do ponto com a respectiva luz
		if (isAffectedByLight(shadow)){
			float attenuation = 1.0f / (at.x + at.y * lightDist + at.z * lightDist * lightDist);
			color = ((oB->Get_k_constants().x * glm::dot(L, normal)) +
				(oB->Get_k_constants().y * pow(glm::dot(H, normal), oB->Get_k_constants().z) * attenuation)) * lighColor * oB->GetFillColor(point, normal);
		}
	}

	return color;
}


//Calcular Raios Sombra
glm::vec3 RayTracing::shade(Object * oB, glm::vec3 normal, glm::vec3 point){
	glm::vec3 color;
	std::vector<Light*> lights = Scene::getInstance().GetLights();

	if (lights.size() > 0)
		color = glm::vec3(0);
	else
		return oB->GetFillColor(point, normal);

	//Vai calcular a sombra para cada fonte de luz
	for (std::vector<Light*>::iterator il = lights.begin(); il != lights.end(); il++){
		for (int x = 0; x < LIGHT_SAMPLE; x++){
			for (int y = 0; y < LIGHT_SAMPLE; y++){
				glm::vec3 new_position = (*il)->position;
				//Se houver mais do que uma luz faz Jittering
				if (LIGHT_SAMPLE > 1){
					new_position.x += -LIGHTS_AREA + (x * 2.0f * LIGHTS_AREA / LIGHT_SAMPLE) + (1.0f / ((rand() % 100) + 1.0f) * (2.0f * LIGHTS_AREA / LIGHT_SAMPLE));
					new_position.y += -LIGHTS_AREA + (y * 2.0f * LIGHTS_AREA / LIGHT_SAMPLE) + (1.0f / ((rand() % 100) + 1.0f) * (2.0f * LIGHTS_AREA / LIGHT_SAMPLE));
				}
				color += calculateShadow(new_position, (*il)->color, point, normal, oB) / (LIGHT_SAMPLE * LIGHT_SAMPLE);
			}
		}
	}

	return color;
}


//Calcular Raios de Reflexao
void RayTracing::calculateReflection(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color){
	if (oB->Get_k_constants().y != 0){
		glm::vec3 reflected_color;
		glm::vec3 R = ray.D - 2 * glm::dot(ray.D, normal) * normal; //Raio de reflexao
		Ray reflected_ray;
		reflected_ray.O = point + 0.001f*R;
		reflected_ray.D = R;
		reflected_color = rayTracing(reflected_ray, depth + 1, ior);
		color += reflected_color * oB->Get_k_constants().y;
	}
}


//Calcular Raios de Refraccao
void RayTracing::calculateRefraction(Ray ray, Object * oB, glm::vec3 point, glm::vec3 normal, int depth, float ior, glm::vec3 &color){
	if (oB->getTransmittance() != 0){
		glm::vec3 refracted_color;

		// Ver questão do sinal do ray.D	
		glm::vec3 vt = glm::dot(-ray.D, normal) * normal + ray.D;
		float sin_teta_i = Utils::norma(vt);

		// Ver se está dentro ou fora do objecto
		float sin_teta_t;
		float new_reflected_index;

		if (ior != 1) //de dentro para fora
			new_reflected_index = 1;
		else //de fora para dentro
			new_reflected_index = oB->getRefractionIndex();

		//Lei snell
		sin_teta_t = ior / new_reflected_index * sin_teta_i;

		//Se se verificar não houve reflexao perfeita
		if (sin_teta_t*sin_teta_t <= 1){
			float cos_teta_t = sqrt(1 - (sin_teta_t * sin_teta_t));
			glm::vec3 t = glm::normalize(vt);
			glm::vec3 rt = sin_teta_t*t + cos_teta_t * (-normal); //Raio de refraccao
			rt = glm::normalize(rt);
			Ray refracted_ray;
			refracted_ray.O = point + 0.001f*rt;
			refracted_ray.D = rt;
			refracted_color = rayTracing(refracted_ray, depth + 1, new_reflected_index);
			color += refracted_color * oB->getTransmittance();
		}
	}
}


//Ray Tracing
glm::vec3 RayTracing::rayTracing(Ray ray, int depth, float ior){
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 color(0);

	//Objecto mais proximo
	Object * oB = Scene::getInstance().GetNearestObject(ray, point, normal);

	//Se nao existir uma intercepcao deve ser dada a cor do background
	if (oB == NULL)
		return Scene::getInstance().GetBckgColor();

	//Se ainda nao atingimos o limite temos de calcular os raios secundarios
	if (depth < MAX_DEPTH){

		//Refleccao
		glm::vec3 r1Color;
		calculateReflection(ray, oB, point, normal, depth, ior, r1Color);

		//Refraccao
		glm::vec3 r2Color;
		calculateRefraction(ray, oB, point, normal, depth, ior, r2Color);

		color += r1Color + r2Color;
	}

	//Cor final
	return color + shade(oB, normal, point);
}
