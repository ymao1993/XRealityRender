#include "../XRCommon.h"
#include "BoxFluid.h"
#include "../utils/XRShaderUtils.h"
#include "../XRScene.h"

#include "../XRSound.h"
#include "../effect/EffectPhongLightingGS.h"


bool BoxFluid::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up model2world transformation
	model2World = glm::translate(glm::vec3(-2, -2.5, 0)) *glm::scale(glm::vec3(0.1, 0.1, 0.1));

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
	//XREffect *effect = new EffectPhongLightingGS();
	//this->addComponent(effect);

	//set up simulator
	{
		SPHSim::SPHConfig conf;
		conf.kernel_h = KERNEL_H;
		conf.k = K;
		conf.density0 = RESET_DENSITY;
		conf.g = G;
		conf.miu = MIU;
		conf.boundarydamping = BOUNDARYDAMPING;
		conf.damping = DAMPING;
		conf.tensionCoe = TENSION_COEF;
		conf.radius = RADIUS;
		conf.cube_h = CUBE_H;
		conf.boxWidth = BOX_WIDTH;
		conf.boxHeight = BOX_HEIGHT;
		conf.boxDepth = BOX_DEPTH;
		conf.boxBasex = BOX_BASE_X;
		conf.boxBasey = BOX_BASE_Y;
		conf.boxBasez = BOX_BASE_Z;
		simulator = new SPHSim::SPHSimulator(conf);
		simulator->setup();
		simulator->update(0);
	}

	//update mesh
	{
		XRMesh * mesh = (XRMesh*) this->getComponent(XRComponentType::XR_COMPONENT_MESH);
		simulator->getData(&mesh->positions, &mesh->normals, mesh->vertexNum);

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
		simulator->setup();
	}
	//update simulator
	simulator->update(1.f / 60.f);

	// handle collision
	{
		XRMesh* boundMesh = (XRMesh*) this->scene->getObject("Kitchen")->getComponent(XRComponentType::XR_COMPONENT_MESH);
		simulator->collisionHandle(this, this->scene->getObject("Kitchen"));
	}

	//update mesh
	{
		XRMesh* mesh = (XRMesh*) this->getComponent(XRComponentType::XR_COMPONENT_MESH);
		simulator->getData(&mesh->positions, &mesh->normals, mesh->vertexNum);
		for (int i = 0; i < mesh->vertexNum; i++)
		{
			vec4 worldPoint = model2World * vec4(mesh->positions[3 * i], mesh->positions[3 * i + 2], mesh->positions[3 * i + 1], 1);
			worldPoint /= worldPoint.w;
			this->scene->brush.drawPoint(vec3(worldPoint), true);
		}
	}


	return true;
}

bool BoxFluid::destroyObject()
{
	delete simulator;
	return true;
}
