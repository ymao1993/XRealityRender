#ifndef PHONGLIGHTINGGS
#define PHONGLIGHTINGGS

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#define XR_ASSIGN_GLM(NAME) \
	memcpy(this->NAME, glm::value_ptr(NAME), sizeof(this->NAME))
#define XR_ASSIGN(NAME) \
	this->NAME = NAME;

#pragma region uniform block structs
/*uniform block structs are packed according to std140 specification*/

//layout(std140, binding = 1) uniform Material{
//	vec3  ambient;
//	vec3  diffuse;
//	vec3  specular;
//	float spower;
//}material;
typedef struct _Material
{
	float ambient[3];	float _pad1;
	float diffuse[3];	float _pad2;
	float specular[3];  float spower;


	_Material(const glm::vec3 &ambient,
		const glm::vec3 &diffuse,
		const glm::vec3 &specular,
		float spower)
	{
		XR_ASSIGN_GLM(ambient);
		XR_ASSIGN_GLM(diffuse);
		XR_ASSIGN_GLM(specular);
		XR_ASSIGN(spower);
	}
	_Material(){}

}Material;

//layout(std140, binding = 0) uniform PointLight{
//	vec3  pos;
//	vec3  ambient;
//	vec3  diffuse;
//	vec3  specular;
//}pointLight;
typedef struct _PointLight
{
	float pos[3];		float _pad0;
	float ambient[3];	float _pad1;
	float diffuse[3];	float _pad2;
	float specular[3];  float _pad3;

	_PointLight(const glm::vec3 &pos, 
				const glm::vec3 &ambient, 
				const glm::vec3 &diffuse, 
				const glm::vec3 &specular) 
	{
		XR_ASSIGN_GLM(pos);
		XR_ASSIGN_GLM(ambient);
		XR_ASSIGN_GLM(diffuse);
		XR_ASSIGN_GLM(specular);
	}
	_PointLight(){}
}PointLight;

#pragma endregion


#endif