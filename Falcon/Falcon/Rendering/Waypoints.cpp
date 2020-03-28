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
	for(float t = 0.0f; t < 1.0f; t += sampleRate)
	{
		float s = 1 - t;
		glm::vec3 a = pow(s, 3.0f) * startPoint;
		glm::vec3 b = 3 * pow(s, 2.0f) * t * controlB;
		glm::vec3 c = 3 * s * pow(t, 2.0f) * controlC;
		glm::vec3 d = pow(t, 3.0f) * nextPoint;
		m_points.push_back(a + b + c + d);
		//glm::vec3 ab = startPoint * s + controlB * t;
		//glm::vec3 bc = controlB * s + controlC * t;
		//glm::vec3 cd = controlC * s + nextPoint * t;
		//glm::vec3 abc = ab * s + bc * t;
		//glm::vec3 bcd = bc * s + cd * t;
		//m_points.push_back(abc * s + bcd + t);
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