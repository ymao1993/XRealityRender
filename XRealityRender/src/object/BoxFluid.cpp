#include "../gl3w/gl3w.h"
#include "BoxFluid.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../XRSound.h"
#include "../effect/EffectPhongLightingGS.h"


bool BoxFluid::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up model2world transformation
	model2World = glm::translate(glm::vec3(-2,-5,0)) *glm::scale(glm::vec3(0.1, 0.1, 0.1));

	//set up mesh
	XRMesh *mesh = new XRMesh();
	this->addComponent(mesh);

	//set up material
	XRMaterial* material = new XRMaterial(
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.8, 0.8, 0.8),
		40);
	this->addComponent(material);

	//set up effect
	XREffect *effect = new EffectPhongLightingGS();
	this->addComponent(effect);

	//set up simulator
	simulator.setup();
	simulator.update(0);

	//update mesh
	{
		XRMesh * mesh = (XRMesh*) this->getComponent(XRComponentType::XR_COMPONENT_MESH);
		simulator.getData(&mesh->positions, &mesh->normals, mesh->vertexNum);
	}
	
	for (int i = 0; i < mesh->vertexNum; i++)
	{
		printf("(%.2lf,%.2lf,%.2lf)\n", mesh->positions[3 * i + 0], mesh->positions[3 * i + 1], mesh->positions[3 * i + 2]);
	}

	return true;
}

bool BoxFluid::updateObject(double time)
{
	static int count = 0;
	count++;
	if (count == 400)
	{
		count = 0;
		simulator.setup();
	}
	//update simulator
	simulator.update(1.0/60.0);
	//update mesh
	{
		XRMesh * mesh = (XRMesh*) this->getComponent(XRComponentType::XR_COMPONENT_MESH);
		simulator.getData(&mesh->positions, &mesh->normals, mesh->vertexNum);
	}
	return true;
}

bool BoxFluid::destroyObject()
{
	return true;
}
