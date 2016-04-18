#ifndef SPHCOMMOM_H
#define SPHCOMMOM_H

#include <vector>
#include "SPHMath.h"

#define KERNEL_H 		(1.1f)
#define K 				(1e3)
#define RESET_DENSITY   (1.2f) 
#define G 				(9.8f)
#define MIU				(0.1f)
#define BOUNDARYDAMPING (0.6)
#define DAMPING 		(0.1f)
#define TENSION_COEF	(1.f)

#define RADIUS 			(1.f)
#define CUBE_H  		(0.5f)
#define BOX_WIDTH		(40)
#define BOX_HEIGHT		(40)
#define BOX_DEPTH 		(40)
#define BOX_BASE_X		(0)
#define BOX_BASE_Y 		(0)
#define BOX_BASE_Z 		(0)
	
namespace SPHSim
{
	/*
	 * SPH Particle struct
	 **/

	struct SPHParticle
	{
		float mass;
		float pressure;
		float density;
		vec3 velocity; 
		vec3 position;
		vec3 f; //total forces
		std::vector<int> neighbors;
		SPHParticle(float mass, vec3 position):mass(mass),position(position){}
	};

	struct UnitCube
	{
		bool vertexFlag[8];
		SPHSim::vec3 cubePos;

		inline SPHSim::vec3 addOffset(double dx, double dy, double dz) 
		{
			return cubePos + SPHSim::vec3(dx, dy, dz);
		}
	};

	struct Grid
	{
		bool flag;
		SPHSim::vec3 position;
		std::vector<int> particleIndices;
	};

	struct Vertex 
	{
	SPHSim::vec3 position;
	SPHSim::vec3 normal;
	};
}
#endif