#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Light {
public:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	Light(vec3 a, vec3 d, vec3 s) : ambient(a), diffuse(d), specular(s)
	{

	}
};

#endif