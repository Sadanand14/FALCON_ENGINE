#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <Types.h>
#include <Memory/fmemory.h>
#include <boost/container/vector.hpp>

class Waypoint
{
	private:
		boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>> m_points;
		int m_curWaypoint;

	public:
		Waypoint();

		void AddWaypoint(glm::vec3 point);
		glm::vec3 NextWaypoint();
		glm::vec3 GetCurrentWaypoint();
};

#endif
