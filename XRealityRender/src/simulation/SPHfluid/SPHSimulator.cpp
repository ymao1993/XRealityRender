#include "SPHSimulator.h"
#include "SPHCommon.h"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <algorithm>

typedef std::chrono::high_resolution_clock Clock;

#define SPH_TEST
#ifdef SPH_TEST
#define WIDTH		20
#define HEIGHT		20
#define DEPTH		20
#define SCALE		1.0f
#define STARTX		10.f
#define STARTY		10.f
#define STARTZ 		15.0f
#endif


namespace SPHSim
{
	SPHSimulator::SPHSimulator() :
		particleGrid(0, 0, 0, BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH, KERNEL_H),
		marchCube(0, 0, 0, BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH, CUBE_H)
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
		for (int i = 0; i<WIDTH; i++)
		{
			float x = STARTX + SCALE * i;
			for (int j = 0; j<HEIGHT; j++)
			{
				float y = STARTY + SCALE * j;
				for (int k = 0; k<DEPTH; k++)
				{
					float z = STARTZ + SCALE * k;
					particles.push_back(SPHParticle(mass, vec3(x, y, z)));
				}
			}
		}

		/*reference parameters*/
		conf.h = KERNEL_H;
		conf.k = K;
		conf.density0 = RESET_DENSITY;
		conf.g = G;
		conf.miu = MIU;
		conf.boundarydamping = BOUNDARYDAMPING;
		conf.damping = DAMPING;
		conf.tensionCoe = TENSION_COEF;
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
		if (r <= h)
		{
			result = (315.f / (64.f * PI * pow(h, 9))) * pow(pow(h, 2) - pow(r, 2), 3);
		}
		return result;
	}

	inline vec3 SPHSimulator::wPoly6Gradient(vec3 rvec, float h)
	{
		float r = rvec.norm();
		vec3 result;
		if (r <= h)
		{
			result = -945.0f / (32.0f * PI * std::pow(h, 9)) * std::pow(std::pow(h, 2) - std::pow(r, 2), 2) * rvec;
		}
		return result;
	}

	inline float SPHSimulator::wPoly6Laplacian(vec3 rvec, float h)
	{
		float r = rvec.norm();
		float result = 0;
		if (r <= h)
		{
			result = 945.0f / (32.0f * PI * std::pow(h, 9)) * (std::pow(h, 2) - std::pow(r, 2)) * (7.0f * std::pow(r, 2) - 3.0f * std::pow(h, 2));
		}
		return result;
	}

	inline vec3 SPHSimulator::wSpikyGradient(vec3 rvec, float h)
	{
		float r = rvec.norm();
		vec3 result;
		if (r <= h && r > EPSILON)
		{
			result = ((-45.f * std::pow(h - r, 2)) / (r * PI * std::pow(h, 6))) * rvec;
		}
		return result;
	}

	inline float SPHSimulator::wViscosityLaplacian(vec3 rvec, float h)
	{
		float r = rvec.norm();
		float result = 0;
		if (r <= h && r>EPSILON)
		{
			result = 45.f / (PI * std::pow(h, 6)) * (h - r);
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

		auto t2_sm = Clock::now();
		std::cout << "Delta particleGrid : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2_sm - t1_sm).count()
			<< " ms" << std::endl;


		//TODO: for all particles, compute density and pressure
		auto t1_sph = Clock::now();
		for (int i = 0; i<particles.size(); i++)
		{
			float density = 0;;
			for (int j : particles[i].neighbors)
			{
				vec3 rvec = particles[i].position - particles[j].position;
				float w = wPoly6(rvec, conf.h);
				density += particles[j].mass * w;
			}
			float pressure = conf.k * (density - conf.density0); //max(xxx,0)??
			particles[i].density = density;
			particles[i].pressure = pressure;
		}

		//TODO: for all particles, compute forces
		for (int i = 0; i<particles.size(); i++)
		{
			vec3 fpressure, fviscosity, ftension, fgravity;
			float color_laplacian = 0;
			vec3 color_gradient;
			for (auto j : particles[i].neighbors)
			{
				vec3 rvec = particles[i].position - particles[j].position;

				//pressure force
				vec3 wspikyGradient = wSpikyGradient(rvec, conf.h);
				fpressure -= (particles[j].mass / particles[j].density) * ((particles[i].pressure + particles[j].pressure) / 2.f) * wspikyGradient;

				//viscosity force
				float wviscosityLaplacian = wViscosityLaplacian(rvec, conf.h);
				fviscosity += conf.miu * (particles[j].mass / particles[j].density) * (particles[j].velocity - particles[i].velocity) * wviscosityLaplacian;

				//compute gradient of color field
				vec3 wpoly6gradient = wPoly6Gradient(rvec, conf.h);
				color_gradient += (particles[j].mass / particles[j].density) * wpoly6gradient;

				//compute laplacian of color filed
				float wpoly6Laplacian = wPoly6Laplacian(rvec, conf.h);
				color_laplacian += (particles[j].mass / particles[j].density) * wpoly6Laplacian;
			}
			fgravity = vec3(0, 0, -conf.g) * particles[i].density;

			if (color_gradient.norm() > EPSILON)
			{
				ftension = -conf.tensionCoe * color_laplacian * color_gradient.unit();
			}

			vec3 f = fpressure + fviscosity + fgravity + ftension;
			particles[i].f = f;
		}

		//TODO: for all particles, update velocity and position
		//using symplectic euler scheme
		for (int i = 0; i<particles.size(); i++)
		{
			vec3 accel = (particles[i].f / particles[i].density);
			accel -= conf.damping * particles[i].velocity; //the faster the velocity, the quicker the damping
			particles[i].velocity += delta * accel;
			particles[i].position += delta * particles[i].velocity;

			//handle boundary condition
			vec3 min(0, 0, 0);
			vec3 max(40, 40, 40);
			box_clamp_and_reflect(particles[i].position, particles[i].velocity, min, max, conf.boundarydamping);
		}
		auto t2_sph = Clock::now();
		std::cout << "Delta SPH : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2_sph - t1_sph).count()
			<< " ms" << std::endl;

		// Marching Cube
		auto t1_mc = Clock::now();
		marchCube.reconstruct(particles, vertices, indices);
		auto t2_mc = Clock::now();
		std::cout << "Marching Cube Delta: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2_mc - t1_mc).count()
			<< " ms" << std::endl;

		std::cout << vertices.size() / 3 << "vertices" << std::endl;
		std::cout << indices.size() / 3 << " triangles" << std::endl;
		//And we are done!
	}

	void SPHSimulator::output(int i)
	{
		std::string filename = "output/frame_" + std::to_string(i) + "_point.dat";
		std::ofstream ofs(filename);
		char line[1024];
		for (int i = 0; i<particles.size(); i++)
		{
			sprintf(line, "%.5f, %.5f, %.5f\n", particles[i].position.x, particles[i].position.y, particles[i].position.z);
			ofs << line;
		}
		ofs.close();

		std::string filename2 = "output/frame_" + std::to_string(i) + "_vertices.dat";
		std::ofstream ofs2(filename2);
		char line2[1024];
		for (int i = 0; i<vertices.size(); i++)
		{
			sprintf(line2, "%.5f, %.5f, %.5f\n", vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			ofs2 << line2;
		}
		ofs2.close();

		std::string filename3 = "output/frame_" + std::to_string(i) + "_indices.dat";
		std::ofstream ofs3(filename3);
		char line3[1024];
		for (int i = 0; i<indices.size() / 3; i++)
		{
			sprintf(line3, "%d, %d, %d\n", indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]);
			ofs3 << line3;
		}
		ofs3.close();
	}

	void SPHSimulator::getData(float** position, float** normal, int& vertexNum)
	{
		//free the space if needed
		if (*position != NULL)
			free(*position);
		if (*normal != NULL)
			free(*normal);

		//allocate new memory
		*position = (float*)malloc(sizeof(float)* indices.size() * 3);
		*normal = (float*)malloc(sizeof(float)* indices.size() * 3);
		vertexNum = indices.size();

		for (int i = 0; i<vertexNum; i++)
		{
			(*position)[3 * i + 0] = vertices[indices[i]].position.x;
			(*position)[3 * i + 1] = vertices[indices[i]].position.z;
			(*position)[3 * i + 2] = vertices[indices[i]].position.y;

			(*normal)[3 * i + 0] = vertices[indices[i]].normal.x;
			(*normal)[3 * i + 1] = vertices[indices[i]].normal.z;
			(*normal)[3 * i + 2] = vertices[indices[i]].normal.y;
		}
		return;
	}

}