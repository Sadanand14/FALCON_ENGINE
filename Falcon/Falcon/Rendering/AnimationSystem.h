#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <cstdlib>
#include <boost/container/vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Ozz stuff
#include <ozz/base/platform.h>
#include <ozz/base/log.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>

//#include <Core/AnimationEvent.h> //This will be done later
#include <Core/EventManager.h>
#include "stb_image.h"
#include <Core/EntityInterface.h>
#include <Core/AssetManager.h>
#include <System/ThreadPool.h>

/**
 * Event system for animation systems
 */
class AnimationEventSystem : public EventSystem
{
private:
	static AnimationEventSystem* m_instance;
	AnimationEventSystem();

public:
	static AnimationEventSystem* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new AnimationEventSystem();
			//m_instance = fmemory::fnew<AnimationEventSystem>();
		}
		return m_instance;
	}

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();

};


/**
 * System that manages animation components.
 */
class AnimationSystem
{
	Entity* entity;
	boost::container::vector<AnimationComponent*> components;
	//AnimationEventSystem* m_eventSystem;

public:
	AnimationSystem();
	~AnimationSystem();

	void Init();
	void Update();

};

#endif // !RENDERER_H

