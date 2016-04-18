/*
 * Marching Cube algorithm reconstruct mesh from point cloud 
 * marching cube impelmenetaion refernce from https://github.com/nsf/mc 
 *
 * Che-Yuan Liang Apr. 2016
 */

#ifndef MARCHCUBE_H
#define MARCHCUBE_H

#include <vector>

#include "SPHMath.h"
#include "SPHCommon.h"
#include "SpatialGrid.h"

class MarchCube: public SpatialGrid
{
private:
	void findNeighbors(int i);
	int  computeCorner(SPHSim::UnitCube &unitCube, const int index);
	void computeNormal(int va_idx, int vb_idx, int vc_idx,
					   std::vector<SPHSim::Vertex> &vertices);
	void computeGrid(const std::vector<SPHSim::SPHParticle> &particles, const int thisGridIndex);


public:
	void reconstruct(const std::vector<SPHSim::SPHParticle> &particles,
		 		 std::vector<SPHSim::Vertex> &vertices, std::vector<int> &indices);

	MarchCube(double bx, double by, double bz, double lx, double ly, double lz, double h, double r)
	: SpatialGrid(bx, by, bz, lx, ly, lz, h, r)
	{
		
	};

	~MarchCube()
	{
		
	};


};


#endif