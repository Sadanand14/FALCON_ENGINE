#include "Renderer.h"

#include <framework.h>
#include <Memory/fmemory.h>

//Included files
//Camera
#include <Camera.h>

//Render passes
#include "RenderPass.h"
#include "MeshRenderPass.h"
#include "ParticleRenderPass.h"
#include "TransparentRenderPass.h"
#include "CanvasRenderPass.h"
#include "SkyRenderPass.h"
#include "QuadRenderPass.h"

//Events
#include <Events/PassToRenderer.h>
#include <Events/EventManager.h>
#include <Events/PassingMeshEvent.h>

//Entities
#include <EntityInterface.h>

//TODO: REMOVE
#include "Canvas.h"
#include "CanvasItems/Label.h"
#include "CanvasItems/Button.h"
#include "CanvasItems/Image.h"
#include "CanvasItems/Panel.h"
#include "CanvasItems/Slider.h"

GLFWwindow* Renderer::m_win = nullptr;

RenderEventSystem* RenderEventSystem::m_instance = nullptr;

/**
*Render Event System Constructor.
*/
RenderEventSystem::RenderEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	//std::cout << "RenderEventSystem intialized with address" << this << "\n";
	subscribedList.push_back(EVENT_RENDER);
	subscribedList.push_back(EVENT_DATA_TO_RENDERER);
	SubscribeToEvents();
}

void RenderEventSystem::ShutDown()
{
	delete m_instance;
}

/**
* Function to process all the events available in the event queue.
*/
void RenderEventSystem::ProcessEvents()
{
	unsigned int count = 0;
	//FL_ENGINE_WARN("eventQueue Size: {0}. \n", eventQueue.size());
	//std::cout << "eventQueue Size: " << eventQueue.size()<<"\n";
	//count++;
	while (!eventQueue.empty())
	{
		boost::shared_ptr<Event> temp = eventQueue.front();
		eventQueue.pop_front();

		//for Data Transfer Events
		if (temp->CheckCategory(EVENT_DATA_TO_RENDERER))
		{
			FL_ENGINE_ERROR("RECIEVED THE MESH DATA!!");
			boost::shared_ptr<PassToRenderer> dataEvent = boost::static_pointer_cast<PassToRenderer>(temp);
			m_skyMesh = dataEvent->GetSkyMesh();
			m_terrainMesh = dataEvent->GetTerrainMesh();
		}
	}
}

/**
 * Function to Subscribe to all the event types listed in the local subscribe list.
 */
void RenderEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

//test function
void RenderEventSystem::PrintReception()
{
	//FL_GAME_INFO("Event Executed using Job system.");
	//std::cout<<"Event Executed SuccessFully on thread :"<< boost::this_thread::get_id()<<"\n";
}
//////////////////////

/**
*Constructor for Renderer
*/
Renderer::Renderer()
{
}

/**
*Destructor for Renderer
*/
Renderer::~Renderer()
{
	fmemory::fdelete(m_UI);
	

	for (auto pass : m_Menu_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			fmemory::fdelete(crp);
		else
			fmemory::fdelete(pass);
	}

	for (auto pass : m_Game_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			fmemory::fdelete(crp);
		else
			fmemory::fdelete(pass);
	}

	RenderEventSystem::ShutDown();
}

/**
*Initialization function for Renderer
*/
void Renderer::Init(GLFWwindow* window)
{
	m_UI = fmemory::fnew<UI::UI_Manager>();
	m_window = window;
	m_RES = RenderEventSystem::GetInstance();
	m_RES->ProcessEvents();
}

/**
*Function to Create Buffers or Programs to be used in the next Draw cycle
*/
void Renderer::CreateDrawStates(GLFWwindow* win)
{
	m_RES->ProcessEvents();
	m_win = win;
	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//m_RES->ProcessEvents();
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entities, glm::mat4 projection)
{
	m_skyMesh = m_RES->GetSkyMesh();
	m_terrainMesh = m_RES->GetTerrainMesh();
	m_projection = projection;

	
	//Menu RenderPasses
	m_Menu_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(0));
	m_Menu_renderPasses[0]->QueueRenderable(m_UI->GetCanvas());

	//Game RenderPasses
	m_Game_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_Game_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_Game_renderPasses.push_back(fmemory::fnew<SkyRenderPass>(2));
	m_Game_renderPasses.push_back(fmemory::fnew<TransparentRenderPass>(3));
	//m_Game_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(4));

	//m_Game_renderPasses[4]->QueueRenderable(m_UI->GetCanvas());


	////First Layer setup
	//m_UI->AddImage("FIRST_PAGE","start race.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));
	//
	//boost::function<void(void)> f1 = [&]() {m_UI->LoadUI("FIRST_PAGE"); };
	//boost::function<void(void)> f2 = [&]() {m_UI->LoadUI("SECOND_PAGE"); };
	//boost::function<void(void)> f3 = [&]() {m_UI->LoadUI("THIRD_PAGE"); };
	//boost::function<void(void)> f4 = [&]() {m_UI->GetCanvas()->ClearCanvas(); };
	////Next Button
	//m_UI->AddButton("FIRST_PAGE",
	//	glm::vec4(255, 255, 255, 0),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.626, 0.820, 0.22, 0.122),
	//	"",
	//	f2
	//);


	////Second Layer setup
	//m_UI->AddImage("SECOND_PAGE", "choose track_lock.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));

	////next button
	//m_UI->AddButton("SECOND_PAGE",
	//	glm::vec4(255, 255, 255, 0),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.725, 0.883, 0.25, 0.08),
	//	"",
	//	f3
	//);

	////prev button
	//m_UI->AddButton("SECOND_PAGE",
	//	glm::vec4(255, 255, 255, 0),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(glm::vec4(0.025, 0.883, 0.274, 0.081)),
	//	"",
	//	f1);

	////Third Layer Setup
	//m_UI->AddImage("THIRD_PAGE", "tune car_1.jpg", glm::vec4(0.0, 0.0, 1.0, 1.0));

	////next button
	//m_UI->AddButton("THIRD_PAGE",
	//	glm::vec4(255, 255, 255, 0),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.725, 0.883, 0.25, 0.08),
	//	"",
	//	f4
	//);

	////prev button
	//m_UI->AddButton("THIRD_PAGE",
	//	glm::vec4(255, 255, 255, 0),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(255, 255, 255, 255),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(glm::vec4(0.025, 0.883, 0.274, 0.081)),
	//	"",
	//	f2);

	//m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.016, 0.62, 0.22, 0.1),0.0f,1.0f,0.1f);
	//m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.258, 0.62, 0.22, 0.1), 0.0f, 1.0f, 0.1f);
	//m_UI->AddSlider("THIRD_PAGE", glm::vec4(0.5, 0.62, 0.22, 0.1), 0.0f, 1.0f, 0.1f);

	//m_UI->LoadUI("FIRST_PAGE");
}

void Renderer::Pause_Update() 
{

}

void Renderer::Pause_Draw() 
{
	//Swap Buffers
	glfwSwapBuffers(m_window);
}

void Renderer::Menu_Update() 
{
	static_cast<CanvasRenderPass*>(m_Menu_renderPasses[0])->PushInput(m_win);
}

void Renderer::Menu_Draw() 
{
	for (unsigned int i = 0; i < m_Menu_renderPasses.size(); ++i) 
	{
		m_Menu_renderPasses[i]->Render();
	}
	//Swap Buffers
	glfwSwapBuffers(m_window);
}


void Renderer::Ingame_Update(Camera& cam, float dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entities)
{
	//static_cast<CanvasRenderPass*>(m_Game_renderPasses[4])->PushInput(m_win);

	m_RES->ProcessEvents();
	m_entities = entities;
	//FL_ENGINE_INFO("Draw Count : {0}", m_entities->size());

	//for skybox
	Shader* skyShader = m_skyMesh->GetMaterial()->m_shader;
	skyShader->UseShader();
	skyShader->SetMat4("projection", m_projection);
	skyShader->SetMat4("view", cam.GetViewMatrix());

	//for terrain
	Shader* temp = m_terrainMesh->GetMaterial()->m_shader;
	temp->UseShader();
	temp->SetMat4("projection", m_projection);
	temp->SetMat4("view", cam.GetViewMatrix());


	for (unsigned int i = 0; i < m_entities->size(); ++i)
	{
		if (m_entities->at(i)->GetComponent<RenderComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
		}

		if (m_entities->at(i)->GetComponent<ParticleEmitterComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<ParticleEmitterComponent>()->m_particle->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
			shader->SetVec3("camPos", cam.m_Position);
		}
	}
}

void Renderer::Ingame_Draw(Camera& cam)
{
	glDepthMask(true);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_skyMesh != nullptr)
	{
		m_skyMesh->AddWorldMatrix(glm::mat4(0.0f));
		m_Game_renderPasses[2]->QueueRenderable(m_skyMesh);
	}


	m_terrainMesh->AddWorldMatrix(glm::mat4(1.0f));
	m_Game_renderPasses[0]->QueueRenderable(m_terrainMesh);
	boost::container::flat_map<float, int> distanceEntityMap;
	for (u32 i = 0; i < m_entities->size(); i++)
	{
		Transform* trans = m_entities->at(i)->GetTransform();

		RenderComponent* rc = m_entities->at(i)->GetComponent<RenderComponent>();
		if (rc)
		{
			Mesh* m = rc->m_mesh;

			if (!m->GetTransparent())
			{
				m->AddWorldMatrix(trans->GetModel());
				m_Game_renderPasses[0]->QueueRenderable(m);
			}

			else
			{
				float dist = glm::distance(trans->GetRelativePosition(), cam.m_Position);
				distanceEntityMap[dist] = i;
			}
		}

		ParticleEmitterComponent* pec = m_entities->at(i)->GetComponent<ParticleEmitterComponent>();
		if (pec)
		{
			Particle* p = pec->m_particle;
			p->SetWorldMatrix(trans->GetModel());
			for (auto it = pec->m_particleBuffer.begin(); it != pec->m_particleBuffer.end(); it++)
				p->AddParticleData(*it);
			m_Game_renderPasses[1]->QueueRenderable(p);
		}
	}

	uint32_t count = 0;
	//Transparent meshes
	for (auto it = distanceEntityMap.rbegin(); it != distanceEntityMap.rend(); it++)
	{
		RenderComponent* rc = m_entities->at(it->second)->GetComponent<RenderComponent>();
		rc->m_mesh->AddWorldMatrix(m_entities->at(it->second)->GetTransform()->GetModel());
		count++;

		auto next = it;
		next++;

		if (next != distanceEntityMap.rend())
		{
			if (m_entities->at(next->second)->GetComponent<RenderComponent>()->m_mesh != rc->m_mesh)
			{
				m_Game_renderPasses[3]->QueueRenderable(rc->m_mesh);
				static_cast<TransparentRenderPass*>(m_Game_renderPasses[3])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
				count = 0;
			}

			else
				continue;
		}

		else
		{
			m_Game_renderPasses[3]->QueueRenderable(rc->m_mesh);
			static_cast<TransparentRenderPass*>(m_Game_renderPasses[3])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
			count = 0;
		}
	}

	for (u32 i = 0; i < m_Game_renderPasses.size(); i++)
	{
		m_Game_renderPasses[i]->Render();
	}

	//Swap Buffers
	glfwSwapBuffers(m_window);
}