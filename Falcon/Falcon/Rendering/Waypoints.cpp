#include "Waypoints.h"

Waypoint::Waypoint()
{
	m_curWaypoint = -1;
}

void Waypoint::AddWaypoint(glm::vec3 point)
{
	m_points.push_back(point);
}

void Waypoint::AddBezierWaypoint(glm::vec3 nextPoint, glm::vec3 controlB, glm::vec3 controlC, float sampleRate)
{
	glm::vec3 startPoint = m_points.back();
	for(float t = sampleRate; t < 1.0f; t += sampleRate)
	{
		float s = 1 - t;
		glm::vec3 a = s * s * s * startPoint;
		glm::vec3 b = 3 * s * s * t * controlB;
		glm::vec3 c = 3 * s * t * t * controlC;
		glm::vec3 d = t * t * t * nextPoint;
		m_points.push_back(a + b + c + d);
	}

	m_points.push_back(nextPoint);
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