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
		void AddBezierWaypoint(glm::vec3 nextPoint, glm::vec3 controlB, glm::vec3 controlC, float sampleRate);
		glm::vec3 NextWaypoint();
		glm::vec3 GetCurrentWaypoint();
		inline boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>>* GetWaypoints() { return &m_points; }
};

#endif
