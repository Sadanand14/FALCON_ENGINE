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
Button* Renderer::prev = nullptr;
Button* Renderer::next = nullptr;
Image* Renderer::bg = nullptr;
Slider* Renderer::wingAngle = nullptr;
Slider* Renderer::gearRatio = nullptr;
Slider* Renderer::suspension = nullptr;
Texture Renderer::uiPage1;
Texture Renderer::uiPage2;
Texture Renderer::uiPage3;

void Renderer::uiNext0()
{
	next->SetCallback(boost::function<void(void)>(uiNext1));
	prev->SetCallback(boost::function<void(void)>(uiPrev1));
	prev->SetActive(true);
	next->SetBounds(nk_rect(0.725, 0.883, 0.25, 0.08));
	bg->SetImage(uiPage2);
}

void Renderer::uiNext1()
{
	next->SetCallback(boost::function<void(void)>(uiNext2));
	prev->SetCallback(boost::function<void(void)>(uiPrev2));
	bg->SetImage(uiPage3);
	wingAngle->SetActive(true);
	gearRatio->SetActive(true);
	suspension->SetActive(true);
}

void Renderer::uiNext2()
{
	bg->SetActive(false);
	next->SetActive(false);
	//prev->SetCallback(boost::function<void(void)>(uiPrev3));
	prev->SetActive(false);
	wingAngle->SetActive(false);
	gearRatio->SetActive(false);
	suspension->SetActive(false);

	glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Renderer::uiPrev1()
{
	next->SetCallback(boost::function<void(void)>(uiNext0));
	prev->SetActive(false);
	next->SetBounds(nk_rect(0.626, 0.820, 0.22, 0.122));
	bg->SetImage(uiPage1);
}

void Renderer::uiPrev2()
{
	next->SetCallback(boost::function<void(void)>(uiNext1));
	prev->SetCallback(boost::function<void(void)>(uiPrev1));
	bg->SetImage(uiPage2);
	wingAngle->SetActive(false);
	gearRatio->SetActive(false);
	suspension->SetActive(false);
}

void Renderer::uiPrev3()
{
	next->SetCallback(boost::function<void(void)>(uiNext2));
	prev->SetCallback(boost::function<void(void)>(uiPrev2));
	next->SetActive(true);
	prev->SetActive(true);
	bg->SetActive(true);
	wingAngle->SetActive(true);
	gearRatio->SetActive(true);
	suspension->SetActive(true);
	glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

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
	fmemory::fdelete(can);
	fmemory::fdelete(next);
	fmemory::fdelete(prev);
	fmemory::fdelete(bg);
	fmemory::fdelete(wingAngle);
	fmemory::fdelete(gearRatio);
	fmemory::fdelete(suspension);

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

	can = fmemory::fnew<Canvas>();
	can->Setup();

	m_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_renderPasses.push_back(fmemory::fnew<SkyRenderPass>(2));
	m_renderPasses.push_back(fmemory::fnew<TransparentRenderPass>(3));
	m_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(4));

	uiPage1.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/start race.jpg");
	uiPage2.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/choose track_lock.jpg");
	uiPage3.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/tune car_1.jpg");
	bg = fmemory::fnew<Image>();
	bg->SetBounds(nk_rect(0.0, 0.0, 1.0, 1.0));
	bg->SetImage(uiPage1);
	//i->AddChild(b);
	//i->AddChild(l);
	static_cast<Canvas*>(can)->AddCanvasItem(bg);

	next = fmemory::fnew<Button>();
	next->SetColor(nk_rgba(255, 255, 255, 0));
	next->SetNormalButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetHoverButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetActiveButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetBounds(nk_rect(0.626, 0.820, 0.22, 0.122));
	next->SetText(std::string(""));
	next->SetCallback(boost::function<void(void)>(uiNext0));
	static_cast<Canvas*>(can)->AddCanvasItem(next);

	prev = fmemory::fnew<Button>();
	prev->SetColor(nk_rgba(255, 255, 255, 0));
	prev->SetNormalButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetHoverButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetActiveButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetBounds(nk_rect(0.025, 0.883, 0.274, 0.081));
	prev->SetActive(false);
	prev->SetText(std::string(""));
	prev->SetCallback([]() { FL_ENGINE_ERROR("THIS SHOULD NOT BE CALLED"); });
	static_cast<Canvas*>(can)->AddCanvasItem(prev);

	wingAngle = fmemory::fnew<Slider>();
	wingAngle->SetBounds(nk_rect(0.016, 0.62, 0.22, 0.1));
	wingAngle->SetMinValue(0.0f);
	wingAngle->SetMaxValue(1.0f);
	wingAngle->SetStep(0.1f);
	wingAngle->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(wingAngle);

	gearRatio = fmemory::fnew<Slider>();
	gearRatio->SetBounds(nk_rect(0.258, 0.62, 0.22, 0.1));
	gearRatio->SetMinValue(0.0f);
	gearRatio->SetMaxValue(1.0f);
	gearRatio->SetStep(0.1f);
	gearRatio->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(gearRatio);

	suspension = fmemory::fnew<Slider>();
	suspension->SetBounds(nk_rect(0.5, 0.62, 0.22, 0.1));
	suspension->SetMinValue(0.0f);
	suspension->SetMaxValue(1.0f);
	suspension->SetStep(0.1f);
	suspension->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(suspension);

	m_renderPasses[4]->QueueRenderable(can);
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entities, glm::mat4 projection)
{
	m_skyMesh = m_RES->GetSkyMesh();
	m_terrainMesh = m_RES->GetTerrainMesh();
	m_projection = projection;
	//RigidbodyDynamic* vehActor = physics::CreateDynamicRigidActor();
	std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> temp;
	
	//Menu RenderPasses
	m_Menu_renderPasses.push_back(fmemory::fnew<QuadRenderPass>(0));

	//Game RenderPasses
	m_Game_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_Game_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_Game_renderPasses.push_back(fmemory::fnew<SkyRenderPass>(2));
	m_Game_renderPasses.push_back(fmemory::fnew<TransparentRenderPass>(3));
	m_Game_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(4));

	uiPage1.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/start race.jpg");
	uiPage2.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/choose track_lock.jpg");
	uiPage3.textureID = AssetManager::LoadTexture("../Assets/Textures/UI/tune car_1.jpg");
	bg = fmemory::fnew<Image>();
	bg->SetBounds(nk_rect(0.0, 0.0, 1.0, 1.0));
	bg->SetImage(uiPage1);
	//i->AddChild(b);
	//i->AddChild(l);
	static_cast<Canvas*>(can)->AddCanvasItem(bg);

	next = fmemory::fnew<Button>();
	next->SetColor(nk_rgba(255, 255, 255, 0));
	next->SetNormalButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetHoverButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetActiveButtonColor(nk_rgba(255, 255, 255, 0));
	next->SetBounds(nk_rect(0.626, 0.820, 0.22, 0.122));
	next->SetText(std::string(""));
	next->SetCallback(boost::function<void(void)>(uiNext0));
	static_cast<Canvas*>(can)->AddCanvasItem(next);

	prev = fmemory::fnew<Button>();
	prev->SetColor(nk_rgba(255, 255, 255, 0));
	prev->SetNormalButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetHoverButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetActiveButtonColor(nk_rgba(255, 255, 255, 0));
	prev->SetBounds(nk_rect(0.025, 0.883, 0.274, 0.081));
	prev->SetActive(false);
	prev->SetText(std::string(""));
	prev->SetCallback([]() { FL_ENGINE_ERROR("THIS SHOULD NOT BE CALLED"); });
	static_cast<Canvas*>(can)->AddCanvasItem(prev);

	wingAngle = fmemory::fnew<Slider>();
	wingAngle->SetBounds(nk_rect(0.016, 0.62, 0.22, 0.1));
	wingAngle->SetMinValue(0.0f);
	wingAngle->SetMaxValue(1.0f);
	wingAngle->SetStep(0.1f);
	wingAngle->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(wingAngle);

	gearRatio = fmemory::fnew<Slider>();
	gearRatio->SetBounds(nk_rect(0.258, 0.62, 0.22, 0.1));
	gearRatio->SetMinValue(0.0f);
	gearRatio->SetMaxValue(1.0f);
	gearRatio->SetStep(0.1f);
	gearRatio->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(gearRatio);

	suspension = fmemory::fnew<Slider>();
	suspension->SetBounds(nk_rect(0.5, 0.62, 0.22, 0.1));
	suspension->SetMinValue(0.0f);
	suspension->SetMaxValue(1.0f);
	suspension->SetStep(0.1f);
	suspension->SetActive(false);
	static_cast<Canvas*>(can)->AddCanvasItem(suspension);

	m_renderPasses[4]->QueueRenderable(can);


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
	static_cast<CanvasRenderPass*>(m_renderPasses[4])->PushInput(m_win);

	temp += 1.0f * dt;
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