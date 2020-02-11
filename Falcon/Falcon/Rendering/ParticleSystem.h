#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <cstdlib>

#include <Events/EventManager.h>
#include <ThreadPool.h>

class ParticleEvent;
class Entity;
class ParticleEmitterComponent;

/**
 * Class Definition for a Render Event System which will respond to all Render Type Events.
 */
class ParticleEventSystem : public EventSystem
{
private:
	static ParticleEventSystem* m_instance;
	ParticleEventSystem();

public:
	static ParticleEventSystem* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new ParticleEventSystem();
			//m_instance = fmemory::fnew<RenderEventSystem>();
		}
		return m_instance;
	}

	virtual void SubscribeToEvents();

	virtual void ProcessEvents();
};


////////////////////////////////////////////////////////////////////////////////
/**
 * Class Definition for the Particle System Responsible for simulating particles.
 */
class ParticleSystem
{
	ParticleEventSystem* m_PES;

public:
	ParticleSystem();
	~ParticleSystem();

	void Init();
	void Update(float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities);
};

#endif //PARTICLE_SYSTEM_H

