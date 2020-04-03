#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <Types.h>
#include <Components/CameraComponent.h>
#include <EventSystem.h>
#include <EventManager.h>
#include <Memory/fmemory.h>
#include <Memory/PoolAllocator.hpp>
//#include <>

typedef boost::container::vector<CameraComponent*, fmemory::STLAllocator<CameraComponent*>> CamArray;

class CameraEvents :public EventSystem
{
private: 
	friend class CameraSystem;
	virtual void SubscribeToEvents();
	CamArray m_arr;
public:
	virtual void ProcessEvents();
	CameraEvents();
	~CameraEvents();
};



class CameraSystem
{
private: 
	CameraEvents* m_camEvents = nullptr;
	CamArray m_entityCams;
	CameraComponent* m_mainCam = nullptr;
	

public:
	CameraSystem();
	~CameraSystem();
	
	inline glm::mat4 GetView() { return m_mainCam->GetViewMatrix(); }
	inline glm::vec3 GetPos() { return m_mainCam->GetPos(); }
	void Update();
	void SetMainCam(unsigned int index);

};

#endif//DEBUG