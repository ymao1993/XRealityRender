/**
 * the code is modified from https://github.com/462cmu/asst3_pathtracer/blob/master/src/bbox.h
 */

#ifndef AABBOX_H
#define AABBOX_H

#include <utility>
#include <algorithm>
#include "../XRCommon.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>

using namespace glm;

/**
* Axis-aligned bounding box.
* An AABB is given by two positions in space, the min and the max. An addition
* component, the extent of the bounding box is stored as it is useful in a lot
* of the operations on bounding boxes.
*/
struct BBox {

	vec3 corner_max;	    ///< min corner of the bounding box
	vec3 corner_min;	    ///< max corner of the bounding box
	vec3 extent;  ///< extent of the bounding box (min -> max)

	/**
	* Constructor.
	* The default constructor creates a new bounding box which contains no
	* points.
	*/
	BBox() {
		corner_max = vec3(-INF_D, -INF_D, -INF_D);
		corner_min = vec3(INF_D, INF_D, INF_D);
		extent = corner_max - corner_min;
	}

	/**
	* Constructor.
	* Creates a bounding box that includes a single point.
	*/
	BBox(const vec3& p) : corner_min(p), corner_max(p) { extent = corner_max - corner_min; }

	/**
	* Constructor.
	* Creates a bounding box with given bounds.
	* \param min the min corner
	* \param max the max corner
	*/
	BBox(const vec3& min, const vec3& max) :
		corner_min(min), corner_max(max) {
		extent = max - min;
	}

	/**
	* Constructor.
	* Creates a bounding box with given bounds (component wise).
	*/
	BBox(const double minX, const double minY, const double minZ,
		const double maxX, const double maxY, const double maxZ) {
		corner_min = vec3(minX, minY, minZ);
		corner_max = vec3(maxX, maxY, maxZ);
		extent = corner_max - corner_min;
	}

	/**
	* Expand the bounding box to include another (union).
	* If the given bounding box is contained within *this*, nothing happens.
	* Otherwise *this* is expanded to the minimum volume that contains the
	* given input.
	* \param bbox the bounding box to be included
	*/
	void expand(const BBox& bbox) {
		corner_min.x = (std::min)(corner_min.x, bbox.corner_min.x);
		corner_min.y = (std::min)(corner_min.y, bbox.corner_min.y);
		corner_min.z = (std::min)(corner_min.z, bbox.corner_min.z);
		corner_max.x = (std::max)(corner_max.x, bbox.corner_max.x);
		corner_max.y = (std::max)(corner_max.y, bbox.corner_max.y);
		corner_max.z = (std::max)(corner_max.z, bbox.corner_max.z);
		extent = corner_max - corner_min;
	}

	/**
	* Expand the bounding box to include a new point in space.
	* If the given point is already inside *this*, nothing happens.
	* Otherwise *this* is expanded to a minimum volume that contains the given
	* point.
	* \param p the point to be included
	*/
	void expand(const vec3& p) {
		corner_min.x = (std::min)(corner_min.x, p.x);
		corner_min.y = (std::min)(corner_min.y, p.y);
		corner_min.z = (std::min)(corner_min.z, p.z);
		corner_max.x = (std::max)(corner_max.x, p.x);
		corner_max.y = (std::max)(corner_max.y, p.y);
		corner_max.z = (std::max)(corner_max.z, p.z);
		extent = corner_max - corner_min;
	}

	vec3 centroid() const {
		return (corner_min + corner_max) / 2.f;
	}

	/**
	* Compute the surface area of the bounding box.
	* \return surface area of the bounding box.
	*/
	double surface_area() const {
		if (empty()) return 0.0;
		return 2 * (extent.x * extent.z +
			extent.x * extent.y +
			extent.y * extent.z);
	}

	/**
	* Compute the maximum dimension of the bounding box (x, y, or z).
	* \return 0 if max dimension is x,
	*         1 if max dimension is y,
	*         2 if max dimension is z
	* TODO: replace with enum (or #define)
	*  - sure but please make sure indexing with the returned value still works
	*/
	uint8_t max_dimension() const {
		uint8_t d = 0;
		if (extent.y > extent.x) d = 1;
		if (extent.z > extent.y) d = 2;
		return d;
	}

	/**
	* Check if bounding box is empty.
	* Bounding box that has no size is considered empty. Note that since
	* bounding box are used for objects with positive volumes, a bounding
	* box of zero size (empty, or contains a single vertex) are considered
	* empty.
	*/
	bool empty() const {
		return corner_min.x > corner_max.x || corner_min.y > corner_max.y || corner_min.z > corner_max.z;
	}
};

std::ostream& operator<<(std::ostream& os, const BBox& b);


#endif 