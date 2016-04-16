#ifndef SPHSIMULATOR_H
#define SPHSIMULATOR_H

#include "SPHCommon.h"
#include "SpatialGrid.h"
#include "MarchCube.h"

/**
* Fluid simulator based on Smoothed Particle Hydrodynamics(SPH)
*
* References:
*
* Müller M, Charypar D, Gross M. Particle-based fluid simulation for i
* nteractive applications[C]//Proceedings of the 2003 ACM SIGGRAPH/Eurographics
* symposium on Computer animation. Eurographics Association, 2003: 154-159.
* (http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.121.844&rep=rep1&type=pdf)
*
* Lecture notes from CMU 15-467 (by Stelian Coros):
* (http://www.cs.cmu.edu/~scoros/cs15467-s16/lectures/10-fluids1.pdf)
* (http://www.cs.cmu.edu/~scoros/cs15467-s16/lectures/11-fluids2.pdf)
*
* Author: Yu Mao
* Contact: ymao1@andrew.cmu.edu
*/

namespace SPHSim
{

	/**
	* SPH configuration paramters
	*/
	struct SPHConfig
	{
		float h; //compact support of the smooth function
		float k; //(density - density0) * k = pressure
		float density0;
		float g; //gravity constant
		float miu;
		float boundarydamping; //0~1
		float damping;
		float tensionCoe; //tension coefficient

	};

	class SPHSimulator
	{
	public:
		/**
		* constructor
		*/
		SPHSimulator();

		/**
		* destructor
		*/
		~SPHSimulator();

		/**
		* setup the simulator at the first time
		*/
		void setup();

		/**
		* reset the all the particle states
		*/
		void reset();

		/**
		* update
		*/
		void update(float delta);

		/**
		* output function for debug
		*/
		void output(int i);


		/**
		* get data for rendering
		*/
		void getData(float** position, float** normal, int& vertexNum);

	private:
		SpatialGrid particleGrid;
		MarchCube   marchCube;

		std::vector<SPHParticle> particles;
		std::vector<SPHSim::Vertex> vertices;
		std::vector<int> indices;

		SPHConfig conf;

		/*predefined kernels*/
		inline float wPoly6(vec3 rvec, float h);
		inline vec3  wSpikyGradient(vec3 rvec, float h);
		inline float wViscosityLaplacian(vec3 rvec, float h);
		inline vec3  wPoly6Gradient(vec3 rvec, float h);
		inline float wPoly6Laplacian(vec3 rvec, float h);

	};
}


#endif