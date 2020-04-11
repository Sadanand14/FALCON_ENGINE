#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <Types.h>
#include <Components/CameraComponent.h>
#include <EventSystem.h>
#include <EventManager.h>
#include <Memory/fmemory.h>
#include <Memory/PoolAllocator.hpp>
//#include <>

typedef boost::container::vector<CameraComponent*> CamArray;

class CameraSystem;

class CameraEventSystem :public EventSystem
{
private: 
	friend class CameraSystem;
	virtual void SubscribeToEvents();
	
public:
	virtual void ProcessEvents();
	CameraEventSystem();
	~CameraEventSystem();
};



class CameraSystem
{
private: 
	friend class CameraEventSystem;
	static CameraEventSystem* m_camEvents;
	static CamArray m_entityCams;
	static CameraComponent* m_mainCam;
	static bool m_cameraMoveable;
	//static void MoveMainCam();

public:
	CameraSystem();
	~CameraSystem();

	static inline const bool IsMoveable() { return m_cameraMoveable; }
	
	static void Initialize();
	static void ShutDown();

	static inline CameraComponent* GetMain() { return m_mainCam; }
	static inline glm::mat4 GetView() { return m_mainCam->GetViewMatrix(); }
	static inline glm::vec3 GetPos() { return m_mainCam->GetPos(); }
	static void Update(float dt);
	static void SetMainCam(unsigned int index);

};
#endif//DEBUG