#include "Waypoints.h"

Waypoint::Waypoint()
{
	m_curWaypoint = -1;
}

void Waypoint::AddWaypoint(glm::vec3 point)
{
	m_points.push_back(point);
}

glm::vec3 Waypoint::NextWaypoint()
{
	if(m_points.size() > 0)
	{
		if(m_curWaypoint < m_points.size() - 1 && m_curWaypoint != -1)
			m_curWaypoint++;

		else
			m_curWaypoint = 0;
	}

	else
		m_curWaypoint = -1;

	return GetCurrentWaypoint();
}

glm::vec3 Waypoint::GetCurrentWaypoint()
{
	if(m_curWaypoint != -1)
		return m_points[m_curWaypoint];
	else
	{
		FL_ENGINE_ERROR("No waypoints added to system");
		return glm::vec3(0, 0, 0);
	}
}