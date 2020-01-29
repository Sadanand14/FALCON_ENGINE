#include "ParticleSystem.h"
#include <Memory/fmemory.h>
#include <Events/ParticleEvent.h>
#include <EntityInterface.h>
#include <Components/ParticleEmitterComponent.h>

ParticleEventSystem* ParticleEventSystem::m_instance = nullptr;

/**
 * Render Event System Constructor.
 */
ParticleEventSystem::ParticleEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	subcribedList.push_back(ParticleEventCategory);
	SubscribeToEvents();
}

/**
 * Function to process all the events available in the event queue.
 */
void ParticleEventSystem::ProcessEvents()
{
	unsigned int count = 0;
	//while (!eventQueue.empty())
	//{
		//eventQueue.pop_front();
		//std::function<void()>f = std::bind(&, this);
		//m_threadPool->submit<>(f);
	//}
}

/**
 * Function to Subscribe to all the event types listed in the local subscribe list.
 */
void ParticleEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subcribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, ParticleEventCategory);
	}
}

//////////////////////

/**
 * Constructor for Renderer
 */
ParticleSystem::ParticleSystem()
{
	Init();
}

/**
 * Destructor for Renderer
 */
ParticleSystem::~ParticleSystem()
{
	delete m_PES;
}

/**
 * Initialization function for Renderer
 */
void ParticleSystem::Init()
{
	m_PES = ParticleEventSystem::GetInstance();
}

/**
 * Function that provides consistent updates for the next rendering frame.
 *
 *@param[in] An integer indicating width.
 *@param[in] An integer indicating height.
 *@param[in] A float indicating zoom.
 *@param[in] A 4x4 matrix defined in glm library.
 *@param[in] A float indicating delta time for the current frame.
 */

void ParticleSystem::Update(float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	m_PES->ProcessEvents();

	for(u32 i = 0; i < entities->size(); i++)
	{
		ParticleEmitterComponent* emitter = entities->at(i)->GetComponent<ParticleEmitterComponent>();

		if(emitter != nullptr)
		{
			if(emitter->m_active)
			{
				emitter->m_timer += dt;

				if ((!emitter->m_particleBuffer.full() && emitter->m_timer >= (1.0f / emitter->m_emissionRate))
					|| emitter->m_particleBuffer.empty())
				{
					float random = (float)rand() / (float)RAND_MAX;
					glm::vec3 spawnOffset = (random * emitter->m_spawnRange * 2.0f) - emitter->m_spawnRange;

					emitter->m_particleBuffer.push_back(ParticleData{ entities->at(i)->GetTransform()->GetPosition() + spawnOffset,
																  emitter->m_startSize,
																  0.0f, 1.0f,
																}
													   );
					//Increment counters and reset timer
					//this->newest = this->particleIndex++;
					//this->particleCount++;
					emitter->m_timer = 0.0f;
					//FL_ENGINE_INFO("emit");
				}


				emitter->m_particlePlayProgress += dt;
				if (emitter->m_particlePlayProgress > emitter->m_particlePlayTimer && !emitter->m_loop)
					emitter->m_active = false;
			}

			for(auto it = emitter->m_particleBuffer.begin(); it != emitter->m_particleBuffer.end(); it++)
			{
				if(it->m_age < emitter->m_particleLifetime)
				{
					float agePercent = it->m_age / emitter->m_particleLifetime;
					it->m_size = glm::mix(emitter->m_startSize, emitter->m_endSize, agePercent);
					it->m_opacity = glm::mix(1.0f, emitter->m_fadeTo, agePercent);
					it->m_pos = emitter->m_velocity * dt + it->m_pos;
					it->m_age += dt;
				}

				else
				{
					emitter->m_particleBuffer.erase(it);
				}
			}
		}
	}
}