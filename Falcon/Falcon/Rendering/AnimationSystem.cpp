#include "AnimationSystem.h"
#include "Memory/fmemory.h"

AnimationEventSystem* AnimationEventSystem::m_instance = nullptr;

/**
 * Animation event system constructor
 */
AnimationEventSystem::AnimationEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	//subcribedList.push_back(AnimationEventCategory);
	SubscribeToEvents();
}

/**
 * Function to process all the events available in the event queue
 */
void AnimationEventSystem::ProcessEvents()
{
	while(!eventQueue.empty())
	{
		eventQueue.pop_front();
		//std::function<void()>f = std::bind(&AnimationEventSystem::FUNC, this);
		//m_threadPool->submit<>(f);
	}
}

/**
 * Function to subscribe to all the event types listed in the local subscribe list.
 */
void AnimationEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subcribedList.size(); i++)
	{
		//EventManager::SubscribeToEvent(this, AnimationEventCategory);
	}
}

/**
 * Constructor for animation systems
 */
AnimationSystem::AnimationSystem()
{
	Init();
}

/**
 * Destructor for animation systems
 */
AnimationSystem::~AnimationSystem()
{

}

/**
 * Initialization function for animation system
 */
void AnimationSystem::Init()
{
	//m_eventSystem = RenderEventSystem::GetInstance();
}

/**
 * Updates all the animation components
 */
void AnimationSystem::Update()
{
	for(u32 i = 0; i < components.size(); i++) {
		//Sample Animation at specified animation time
		ozz::animation::SamplingJob samplingJob;
		samplingJob.animation = &components[i]->anim->m_anim;
		samplingJob.cache = &components[i]->anim->m_cache;
		samplingJob.ratio = 0.0f;//components[i]->anim->m_controller.time_ratio();
		samplingJob.output = ozz::make_range(components[i]->anim->m_locals);

		if (!samplingJob.Run()) {
			//error running job
		}

		//Convert to model coords
		ozz::animation::LocalToModelJob ltmJob;
		ltmJob.skeleton = &components[i]->anim->m_skel;
		ltmJob.input = ozz::make_range(components[i]->anim->m_locals);
		ltmJob.output = ozz::make_range(components[i]->anim->m_models);

		if (!ltmJob.Run()) {
			//error running job
		}
	}
}