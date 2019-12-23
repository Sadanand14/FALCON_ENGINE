#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BasicComponent.h"
#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


/**
* Stucture to hold Tranform data for each entity.
*/
struct Transform: public BasicComponent
{
private:
	bool m_updateFlag;
	glm::mat4 m_model; //* The model matrix of the transform
	glm::vec3 m_position; //* The position of the Tranform
	glm::vec3 m_scale; //* The scale of the transform
	glm::quat m_rotation; //* The rotation of the transform
	glm::mat4 m_parentMatrix;//matrix of parent in world Space
	/**
	 * Recalculates the world matrix
	 */
	void RecalculateMatrix()
	{
		//m_model = m_parentMatrix *glm::mat4(1.0f) ;
		m_model = glm::translate(glm::mat4(1.0f), m_position);
		m_model *= glm::mat4_cast(m_rotation);
		m_model = glm::scale(m_model, m_scale);
	    m_model = m_parentMatrix * m_model;
		m_updateFlag = false;
	}

public:
	Transform()
		:m_position({ 0.0f, 0.0f, 0.0f }), m_rotation(glm::quat()), m_scale({ 1.0f,1.0f,1.0f }), m_model(1.0f),
		m_updateFlag(true), m_parentMatrix(glm::mat4())
	{
		RecalculateMatrix();
	}
	Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
		: m_position(pos), m_rotation(rot), m_scale(scale), m_updateFlag(true), m_parentMatrix(glm::mat4())
	{
		RecalculateMatrix();
	}

	inline void SetRelativeSpace(glm::mat4 parentMatrix)
	{
		m_parentMatrix = parentMatrix; m_updateFlag = true;
	}

	inline const bool GetFlag() const { return m_updateFlag; }
	inline void SetPosition(const glm::vec3& pos) { m_position = pos; m_updateFlag = true; }// m_updateFlag = true; }
	inline void SetRotation(const glm::quat& rot) { m_rotation = rot; m_updateFlag = true; }//m_updateFlag = true; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; m_updateFlag = true; }//m_updateFlag = true; }

	inline const glm::vec3& GetPosition() const { return m_position; }
	inline const glm::quat& GetRotation() const { return m_rotation; }
	inline const glm::vec3& GetScale() const { return m_scale; }
	inline const glm::mat4& GetModel() const { return m_model; }

	void CheckFlag()
	{
		if (m_updateFlag)
			RecalculateMatrix();
	}
};

#endif // !1
