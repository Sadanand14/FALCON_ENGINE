#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <Types.h>
#include <Memory/fmemory.h>
#include <boost/container/vector.hpp>

class Waypoint
{
	private:
		boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>> m_beziers;
		boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>> m_points;
		int m_curWaypoint;
		float m_sampleRate = 0.1f;

		void UpdateWaypointList();

	public:
		Waypoint();

		void AddWaypoint(glm::vec3 point);
		void AddBezier(glm::vec3 point, glm::vec3 control);
		glm::vec3 NextWaypoint();
		glm::vec3 GetCurrentWaypoint();
		inline void SetCurveSampleRate(float sampleRate) { m_sampleRate = glm::clamp(sampleRate, 0.0f, 1.0f); }
		inline float GetCurveSampleRate() { return m_sampleRate; }
		inline boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>>* GetWaypoints() { return &m_points; }
};

#endif
