#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BasicComponent.h"
#include <boost/container/vector.hpp>
#include <Memory/fmemory.h>
#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_inverse.hpp"

typedef boost::container::vector<unsigned short int, fmemory::STLAllocator<unsigned short int>> IDVector;

/**
* Stucture to hold Transform data for each entity.
*/
struct Transform: public BasicComponent
{
private:
	bool m_updateFlag;
	//m_updated;//m_updated for choosing to recalculate matrix and m_updated for reassigning in octree

	IDVector octreeID;

	glm::vec3 m_front;
	glm::mat4 m_model; //* The model matrix of the transform
	glm::mat3 m_normal; //* The normal matrix of the transform
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
		m_model = glm::translate(m_parentMatrix, m_position);
		m_model *= glm::mat4_cast(m_rotation);
		m_model = glm::scale(m_model, m_scale);
		m_normal = glm::mat3(glm::inverseTranspose(m_model));
		//m_front *= m_rotation;
		//m_updated = true;
		m_updateFlag = false;
	}

public:
	Transform()
		:m_position({ 0.0f, 0.0f, 0.0f }), m_rotation(glm::quat()), m_scale({ 1.0f,1.0f,1.0f }), m_model(1.0f),
		m_normal(1.0f), m_updateFlag(true), m_parentMatrix(glm::mat4()), m_front({1.0f, 0.0f, 0.0f})
	{
		octreeID.reserve(10);
		RecalculateMatrix();
	}

	Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
		: m_position(pos), m_rotation(rot), m_scale(scale), m_updateFlag(true),
		m_parentMatrix(glm::mat4()), m_front({ 1.0f, 0.0f, 0.0f })
	{
		octreeID.reserve(10);
		RecalculateMatrix();
	}

	inline void SetRelativeSpace(glm::mat4 parentMatrix)
	{
		m_parentMatrix = parentMatrix; m_updateFlag = true;
	}

	//for octree Calculations
	void popOTID() { if (octreeID.size() > 0) octreeID.pop_back(); }

	short int topOTID() {
		if (octreeID.size() > 0) return octreeID[octreeID.size() - 1];
		else return -1;
	}

	inline const IDVector& GetOTID() const { return octreeID; }
	inline void ClearOTID() { octreeID.clear(); }
	inline void pushOTID(unsigned short int value) { octreeID.push_back(value); }
	//inline void setUpdatedflag(bool value) { m_updated = value; }
	//inline const bool GetUpdatedflag() const { return m_updated; }
	/////////////////////////////////////

	inline const bool GetFlag() const { return m_updateFlag; }
	inline void SetPosition(const glm::vec3& pos) { m_position = pos; m_updateFlag = true; }// m_updateFlag = true; }
	inline void SetRotation(const glm::quat& rot) { m_rotation = rot; m_updateFlag = true; }//m_updateFlag = true; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; m_updateFlag = true; }//m_updateFlag = true; }

	inline const glm::vec3& GetFront() const { return m_front; }
	inline const glm::vec3& GetPosition() const { return m_position; }
	inline const glm::quat& GetRotation() const { return m_rotation; }
	inline const glm::vec3& GetScale() const { return m_scale; }
	inline const glm::mat4& GetModel() const { return m_model; }
	inline const glm::mat3& GetNormal() const { return m_normal; }

	inline const glm::vec3 GetRotDiff() const 
	{
		static glm::vec3 prevRot = { 0.0f,0.0f,0.0f }; 
		static glm::vec3 diff;
		diff = glm::vec3(0.0f, 0.0f, -1.0f) * m_rotation - prevRot;
		prevRot = glm::vec3(0.0f, 0.0f, -1.0f) * m_rotation;
		return diff;
	}

	inline const glm::vec3 GetRelativePosition() const { return glm::vec3(m_parentMatrix * glm::vec4(m_position, 1.0)); }

	void CheckFlag()
	{
		if (m_updateFlag)
			RecalculateMatrix();
	}
};

#endif // !1
