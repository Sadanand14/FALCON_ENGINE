#include "Waypoints.h"

Waypoint::Waypoint()
{
	m_curWaypoint = -1;
}

void Waypoint::AddWaypoint(glm::vec3 point)
{
	m_beziers.push_back(point);
	m_beziers.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	UpdateWaypointList();
}

void Waypoint::AddBezier(glm::vec3 point, glm::vec3 control)
{
	m_beziers.push_back(point);
	m_beziers.push_back(control);
	UpdateWaypointList();
}

void Waypoint::UpdateWaypointList()
{
	m_points.clear();
	for(u32 i = 0; i < m_beziers.size(); i += 2)
	{
		if(m_beziers[i + 1] != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			glm::vec3 startPoint = m_points.back();

			for(float t = m_sampleRate; t < 1.0f; t += m_sampleRate)
			{
				float s = 1 - t;
				//glm::vec3 a = s * s * s * startPoint;
				//glm::vec3 b = 3 * s * s * t * controlB;
				//glm::vec3 c = 3 * s * t * t * controlC;
				//glm::vec3 d = t * t * t * nextPoint
				glm::vec3 a = m_beziers[i + 1];
				glm::vec3 b = s * s * (startPoint - a);
				glm::vec3 c = t * t * (m_beziers[i] - a);
				m_points.push_back(a + b + c);
			}
		}

		m_points.push_back(m_beziers[i]);
	}
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