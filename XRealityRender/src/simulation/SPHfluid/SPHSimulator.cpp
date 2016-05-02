#include "SPHSimulator.h"
#include <glm/glm.hpp>
#include "../../XRMesh.h"
#include "SPHCommon.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <algorithm>

typedef std::chrono::high_resolution_clock Clock;

#define SPH_TEST
#ifdef SPH_TEST
#define WIDTH		10
#define HEIGHT		10
#define DEPTH		10
#define SCALE		1.0f
#define STARTX	    15.f
#define STARTY		15.f
#define STARTZ 		30.f
#endif


namespace SPHSim
{
	SPHSimulator::SPHSimulator(SPHConfig config)
	:
	conf(config),
	particleGrid(config.boxBasex, config.boxBasey, config.boxBasez, config.boxWidth, 
			config.boxHeight, config.boxDepth, config.kernel_h, config.radius), 
	marchCube(config.boxBasex, config.boxBasey, config.boxBasez , config.boxWidth,
	 		config.boxHeight, config.boxDepth, config.cube_h, config.radius)
	{

	}

	SPHSimulator::~SPHSimulator()
	{

	}

	void SPHSimulator::setup()
	{
#ifdef SPH_TEST
		printf("generate test particles...\n");
		float mass = 1.f;
		for(int i=0; i<WIDTH; i++)
		{
			float x = STARTX + SCALE * i;
			for(int j=0; j<HEIGHT; j++)
			{
				float y = STARTY + SCALE * j;
				for(int k=0; k<DEPTH; k++)
				{
					float z = STARTZ + SCALE * k;
					particles.push_back(SPHParticle(mass, vec3(x,y,z)));
				}
			}
		}

#endif

		return;
	}

	void SPHSimulator::reset()
	{
		return;
	}

	inline float SPHSimulator::wPoly6(vec3 rvec, float h)
	{
		float r = rvec.norm();
		float result = 0;
		if(r<=h)
		{
			result = (315.f/(64.f * PI * pow(h,9))) * pow(pow(h,2) - pow(r,2),3);
		}
		return result;
	}

	inline vec3 SPHSimulator::wPoly6Gradient(vec3 rvec, float h)
	{
		float r = rvec.norm();
		vec3 result;
		if(r<=h)
		{
			result = -945.0f / (32.0f * PI * std::pow(h,9)) * std::pow(std::pow(h,2) - std::pow(r,2),2) * rvec;
		}
		return result;	
	}

	inline float SPHSimulator::wPoly6Laplacian(vec3 rvec, float h)
	{
		float r = rvec.norm();
		float result = 0;
		if(r<=h)
		{
			result = 945.0f / (32.0f * PI * std::pow(h,9)) * (std::pow(h,2) - std::pow(r,2)) * (7.0f * std::pow(r,2) - 3.0f * std::pow(h,2));
		}
		return result;		
	}

	inline vec3 SPHSimulator::wSpikyGradient(vec3 rvec, float h)
	{
		float r = rvec.norm();
		vec3 result;
		if(r<=h && r > EPSILON)
		{
			result = ((- 45.f * std::pow(h-r,2)) / (r * PI * std::pow(h,6))) * rvec;
		}
		return result;
	}

	inline float SPHSimulator::wViscosityLaplacian(vec3 rvec, float h)
	{
		float r = rvec.norm();
		float result = 0;
		if(r<=h && r>EPSILON)
		{
			result = 45.f / (PI * std::pow(h,6)) * (h - r);
		}
		return result;		
	}

	void SPHSimulator::update(float delta)
	{
		//TODO: for all particles, find neighbourhoods

		// Spatial Grid Neibor Finding
		auto t1_sm = Clock::now();
		
		particleGrid.clearGrid();

		for (int i = 0; i < particles.size(); i++) 
		{
			particleGrid.addParticle(particles[i], i);
		}

		for (SPHParticle &p_i : particles) 
		{
			particleGrid.findNeighbors(p_i, particles);
		}

		//auto t2_sm = Clock::now();
		//std::cout << "Delta particleGrid : "
		//		  << std::chrono::duration_cast<std::chrono::milliseconds>(t2_sm-t1_sm).count()
		//		  << " ms" << std::endl;

		//TODO: for all particles, compute density and pressure
		auto t1_sph = Clock::now();
		for(int i=0; i<particles.size(); i++)
		{
			float density = 0;;
			for(int j: particles[i].neighbors)
			{
				vec3 rvec = particles[i].position - particles[j].position;
				float w = wPoly6(rvec, conf.kernel_h);
				density += particles[j].mass * w;
			}
			float pressure = conf.k * (density - conf.density0); //max(xxx,0)??
			particles[i].density = density;
			particles[i].pressure = pressure;
		}

		//TODO: for all particles, compute forces
		for(int i=0; i<particles.size(); i++)
		{
			vec3 fpressure, fviscosity, ftension, fgravity;
			float color_laplacian = 0;
			vec3 color_gradient;
			for(auto j: particles[i].neighbors)
			{
				vec3 rvec = particles[i].position - particles[j].position;

				//pressure force
				vec3 wspikyGradient = wSpikyGradient(rvec, conf.kernel_h);
				fpressure  -= (particles[j].mass / particles[j].density) * ((particles[i].pressure + particles[j].pressure) / 2.f) * wspikyGradient;

				//viscosity force
				float wviscosityLaplacian = wViscosityLaplacian(rvec, conf.kernel_h);
				fviscosity += conf.miu * (particles[j].mass / particles[j].density) * (particles[j].velocity - particles[i].velocity) * wviscosityLaplacian;

				//compute gradient of color field
				vec3 wpoly6gradient = wPoly6Gradient(rvec, conf.kernel_h);
				color_gradient += (particles[j].mass / particles[j].density) * wpoly6gradient;

				//compute laplacian of color filed
				float wpoly6Laplacian = wPoly6Laplacian(rvec, conf.kernel_h);
				color_laplacian += (particles[j].mass / particles[j].density) * wpoly6Laplacian;
			}
			fgravity = vec3(0,0,-conf.g) * particles[i].density;

			if(color_gradient.norm() > EPSILON)
			{
				ftension =  -conf.tensionCoe * color_laplacian * color_gradient.unit();
			}

			vec3 f = fpressure + fviscosity + fgravity + ftension;
			particles[i].f = f;
		}

		//TODO: for all particles, update velocity and position
		//using symplectic euler scheme
		for(int i=0; i<particles.size(); i++)
		{
			vec3 accel = (particles[i].f / particles[i].density);
			accel -= conf.damping * particles[i].velocity; //the faster the velocity, the quicker the damping
			particles[i].velocity += delta * accel;
			particles[i].position += delta * particles[i].velocity;	

			

			//box_clamp_and_reflect(particles[i].position, particles[i].velocity ,min, max, conf.boundarydamping);
		}
		//auto t2_sph = Clock::now();
		//std::cout << "Delta SPH : "
		//		  << std::chrono::duration_cast<std::chrono::milliseconds>(t2_sph-t1_sph).count()
		//		  << " ms" << std::endl;
		
		//// Marching Cube
	 //    	auto t1_mc = Clock::now();
		//marchCube.reconstruct(particles, vertices, indices);
		//auto t2_mc = Clock::now();
		//std::cout << "Marching Cube Delta: "
		//		  << std::chrono::duration_cast<std::chrono::milliseconds>(t2_mc-t1_mc).count()
		//		  << " ms" << std::endl;
		//std::cout << vertices.size()/3 << "vertices" << std::endl;
		//std::cout << indices.size()/3 << " triangles" << std::endl; 
		//And we are done!
	}

	//handle boundary condition
	void SPHSimulator::collisionHandle(XRObject* boxFluid, XRObject* kitchen)
	{
		XRMesh* mesh = (XRMesh*)kitchen->getComponent(XRComponentType::XR_COMPONENT_MESH);
		mat4 inv_kitchenToWorld = glm::inverse(kitchen->model2World);
		for (int i = 0; i < particles.size(); i++)
		{
			glm::vec3 pos = glm::vec3(particles[i].position.x, particles[i].position.z, particles[i].position.y); // flip y, z
			glm::vec4 tmp;
			glm::vec3 normal;
			glm::vec3 nearestSurfPos;
		
			// box to world
			tmp = boxFluid->model2World * glm::vec4(pos.x, pos.y, pos.z, 1);
			tmp /= tmp.w;
			pos = glm::vec3(tmp);
			// world to kitchen
			tmp = inv_kitchenToWorld * glm::vec4(pos.x, pos.y, pos.z, 1);
			tmp /= tmp.w;
			pos = glm::vec3(tmp);

			if (mesh->heightField.collisionDetection(pos, normal, nearestSurfPos))
			{
				tmp = kitchen->model2World * glm::vec4(nearestSurfPos.x, nearestSurfPos.y, nearestSurfPos.z, 1);
				tmp /= tmp.w;
				nearestSurfPos = glm::vec3(glm::inverse(boxFluid->model2World) * tmp);
				// reset position 
				particles[i].position.z = nearestSurfPos.y;
				
				// update collision velocity
				normal = normalize(normal);
				glm::vec3 vXR = glm::vec3(particles[i].velocity.x, particles[i].velocity.z, particles[i].velocity.y);
				vXR = glm::reflect(vXR, normal);
				vXR = vXR - dot(normal, vXR) * normal * float(conf.boundarydamping);
				vec3 new_v = vec3(vXR.x, vXR.z, vXR.y);
				particles[i].velocity = new_v;
			}
		}
	};

	void SPHSimulator::output(int i)
	{
		std::string filename = "output/frame_" + std::to_string(i) + "_point.dat";
		std::ofstream ofs(filename);
		char line[1024];
		for(int i=0; i<particles.size(); i++)
		{
			sprintf(line, "%.5f, %.5f, %.5f\n", particles[i].position.x, particles[i].position.y, particles[i].position.z);
			ofs << line;
		}
		ofs.close();

		std::string filename2 = "output/frame_" + std::to_string(i) + "_vertices.dat";
		std::ofstream ofs2(filename2);
		char line2[1024];
		for (int i=0; i<vertices.size(); i++)
		{
			sprintf(line2, "%.5f, %.5f, %.5f\n", vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			ofs2 << line2;
		}
		ofs2.close();

		std::string filename3 = "output/frame_" + std::to_string(i) + "_indices.dat";
		std::ofstream ofs3(filename3);
		char line3[1024];
		for (int i=0; i<indices.size()/3; i++)
		{
			sprintf(line3, "%d, %d, %d\n", indices[3*i], indices[3*i+1], indices[3*i+2]);
			ofs3 << line3;
		}
		ofs3.close();
	}

	void SPHSimulator::getData(float** position, float** normal, int& vertexNum)
	{
		//free the space if needed
		if(*position != NULL) 
			free(*position);
		if(*normal != NULL)   
			free(*normal);

		////allocate new memory
		//*position = (float*)malloc(sizeof(float) * indices.size() * 3);
		//*normal   = (float*)malloc(sizeof(float) * indices.size() * 3);
		//vertexNum    = indices.size();

		//for(int i=0; i<vertexNum; i++)
		//{
		//	(*position)[3*i+0] = vertices[indices[i]].position.x;
		//	(*position)[3*i+1] = vertices[indices[i]].position.z;
		//	(*position)[3*i+2] = vertices[indices[i]].position.y;

		//	(*normal)[3*i+0] = vertices[indices[i]].normal.x;
		//	(*normal)[3*i+1] = vertices[indices[i]].normal.z;
		//	(*normal)[3*i+2] = vertices[indices[i]].normal.y;
		//}

		// draw point
		*position = (float*)malloc(sizeof(float) * particles.size() * 3);
		*normal = (float*)malloc(sizeof(float)* particles.size() * 3);
		vertexNum = particles.size();
		for (int i = 0; i < particles.size(); i++)
		{
			(*position)[3 * i + 0] = particles[i].position.x;
			(*position)[3 * i + 1] = particles[i].position.y;
			(*position)[3 * i + 2] = particles[i].position.z;

			(*normal)[3 * i + 0] = 1;
			(*normal)[3 * i + 1] = 0;
			(*normal)[3 * i + 2] = 0;
		}
		return;
	}

}
