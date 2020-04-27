#include "Renderer.h"


#include <framework.h>
#include <Memory/fmemory.h>

//Included files
//Camera
//#include <Camera.h>

//Render passes
#include "RenderPass.h"
#include "MeshRenderPass.h"
#include "ParticleRenderPass.h"
#include "TransparentRenderPass.h"
#include "CanvasRenderPass.h"
#include "SkyRenderPass.h"
#include "QuadRenderPass.h"

#if defined(_DEBUG) || defined(DEBUG)
#include "LineRenderPass.h"
#endif //Line debug

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
{}

/**
*Destructor for Renderer
*/
Renderer::~Renderer()
{
	glDeleteTextures(1, &m_brdfTextue.textureID);
	//fmemory::fdelete(m_UI);
	CameraSystem::ShutDown();

	for (auto pass : m_Menu_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			//fmemory::fdelete(crp);
			delete crp;
		else
			//fmemory::fdelete(pass);
			delete pass;
	}

	for (auto pass : m_Game_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			//fmemory::fdelete(crp);
			delete crp;
		else
			//fmemory::fdelete(pass);
			delete pass;
	}

	RenderEventSystem::ShutDown();
}

/**
*Initialization function for Renderer
*/
void Renderer::Init(GLFWwindow* window)
{
	CameraSystem::Initialize();
	//m_UI = fmemory::fnew<UI::UI_Manager>();
	//m_UI = new UI::UI_Manager();
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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//m_RES->ProcessEvents();

	//wp.AddWaypoint(glm::vec3( 0.0f,   0.0f,  0.0f));
	//wp.AddBezier(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, -2.0f));
	////wp.AddWaypoint(glm::vec3(10.0f,   0.0f,  0.0f));
	//wp.AddWaypoint(glm::vec3(20.0f, -5.0f,  0.0f));
	//wp.AddWaypoint(glm::vec3(20.0f, -5.0f, 10.0f));
	//wp.AddWaypoint(glm::vec3(10.0f, -5.0f, 10.0f));
	//wp.AddWaypoint(glm::vec3( 5.0f,  0.0f, 10.0f));

	//Mesh quad;
	//quad.m_vertexArray.push_back(Vertex{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2( 0.0f,  1.0f), glm::vec3(), glm::vec3(), glm::vec3()});
	//quad.m_vertexArray.push_back(Vertex{ glm::vec3(-1.0f, -3.0f, 0.0f), glm::vec2( 0.0f, -1.0f), glm::vec3(), glm::vec3(), glm::vec3()});
	//quad.m_vertexArray.push_back(Vertex{ glm::vec3( 3.0f,  1.0f, 0.0f), glm::vec2( 2.0f,  1.0f), glm::vec3(), glm::vec3(), glm::vec3()});
	//quad.m_indexArray.push_back(0);
	//quad.m_indexArray.push_back(1);
	//quad.m_indexArray.push_back(2);
	//quad.Setup();

	//quad.Bind();

	//Shader* brdfShad = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/brdf.vert", "../Falcon/Rendering/Shader/brdf.frag");

	//unsigned int captureFBO, captureRBO;
	//glGenFramebuffers(1, &captureFBO);
	//glGenRenderbuffers(1, &captureRBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//glGenTextures(1, &m_brdfTextue.textureID);
	//glBindTexture(GL_TEXTURE_2D, m_brdfTextue.textureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glViewport(0, 0, 512, 512);
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfTextue.textureID, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//brdfShad->UseShader();
	////Draw to quad
	//glDrawElements(GL_TRIANGLES, quad.m_indexArray.size(), GL_UNSIGNED_INT, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 1280, 720);

	//glDeleteFramebuffers(1, &captureFBO);
	//glDeleteRenderbuffers(1, &captureRBO);

	//fmemory::fdelete(brdfShad);
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*>* entities, glm::mat4 projection)
{
	m_skyMesh = m_RES->GetSkyMesh();
	m_terrainMesh = m_RES->GetTerrainMesh();
	m_projection = projection;

	CameraSystem::Update(0.016f);

	////Menu RenderPasses
	//m_Menu_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(0));
	//m_Menu_renderPasses[0]->QueueRenderable(m_UI->GetCanvas());

	//Game RenderPasses
	//m_Game_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_Game_renderPasses.push_back(new MeshRenderPass(0));
	//m_Game_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_Game_renderPasses.push_back(new ParticleRenderPass(1));
	m_Game_renderPasses.push_back(new SkyRenderPass(2));
	m_Game_renderPasses.push_back(new TransparentRenderPass(3));
//
//#if defined(_DEBUG) || defined(DEBUG)
//	wpTestLine.Setup();
//	auto points = wp.GetWaypoints();
//
//	for(u32 i = 0; i < points->size(); i++)
//		wpTestLine.AddPoint(points->at(i));
//
//	wpTestLine.SetLineType(LineType::LOOP);
//	wpTestLine.SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
//
//	m_Game_renderPasses.push_back(fmemory::fnew<LineRenderPass>(4));
//	m_Game_renderPasses.back()->QueueRenderable(&wpTestLine);
//#endif //Line debug
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


void Renderer::Ingame_Update(float dt, boost::container::vector<Entity*>* entities)
{
	//static_cast<CanvasRenderPass*>(m_Game_renderPasses[4])->PushInput(m_win);

	m_RES->ProcessEvents();

	CameraSystem::Update(dt);

	m_entities = entities;
	//FL_ENGINE_INFO("Draw Count : {0}", m_entities->size());

	//for skybox
	Material* skyMat = m_skyMesh->GetMaterial();
	Shader* skyShader = skyMat->m_shader;
	skyShader->UseShader();
	skyShader->SetMat4("projection", m_projection);
	skyShader->SetMat4("view", CameraSystem::GetView());

	//for terrain
	Shader* temp = m_terrainMesh->GetMaterial()->m_shader;
	temp->UseShader();
	temp->SetMat4("projection", m_projection);
	temp->SetMat4("view", CameraSystem::GetView());
	temp->SetVec3("camPos", CameraSystem::GetPos());
	temp->SetInt("irradianceMap", 6);
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyMat->m_normalTex.textureID);
	temp->SetInt("prefilterMap", 7);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyMat->m_roughnessTex.textureID);
	temp->SetInt("brdf", 8);
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, m_brdfTextue.textureID);

	for (unsigned int i = 0; i < m_entities->size(); ++i)
	{
		if (m_entities->at(i)->GetComponent<RenderComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", CameraSystem::GetView());
			shader->SetVec3("camPos", CameraSystem::GetPos());
			shader->SetInt("irradianceMap", 6);
			glActiveTexture(GL_TEXTURE0 + 6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyMat->m_normalTex.textureID);
			shader->SetInt("prefilterMap", 7);
			glActiveTexture(GL_TEXTURE0 + 7);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyMat->m_roughnessTex.textureID);
			shader->SetInt("brdf", 8);
			glActiveTexture(GL_TEXTURE0 + 8);
			glBindTexture(GL_TEXTURE_2D, m_brdfTextue.textureID);
		}

		if (m_entities->at(i)->GetComponent<ParticleEmitterComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<ParticleEmitterComponent>()->m_particle->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", CameraSystem::GetView());
			shader->SetVec3("camPos", CameraSystem::GetPos());
		}
	}

//#if defined(_DEBUG) || defined(DEBUG)
//	Shader* lineShader = wpTestLine.GetMaterial()->m_shader;
//	lineShader->UseShader();
//	lineShader->SetMat4("projection", m_projection);
//	lineShader->SetMat4("view", CameraSystem::GetView());
//#endif //Line debug
}

void Renderer::Ingame_Draw()
{
	glDepthMask(true);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_skyMesh != nullptr)
	{
		m_skyMesh->AddWorldAndNormalMatrix(glm::mat4(0.0f), glm::mat3(0.0f));
		m_Game_renderPasses[2]->QueueRenderable(m_skyMesh);
	}

	m_terrainMesh->AddWorldAndNormalMatrix(glm::mat4(1.0f), glm::mat3(1.0f));
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
				m->AddWorldAndNormalMatrix(trans->GetModel(), trans->GetNormal());
				m_Game_renderPasses[0]->QueueRenderable(m);
			}

			else
			{
				float dist = glm::distance(trans->GetRelativePosition(), CameraSystem::GetPos());
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
		rc->m_mesh->AddWorldAndNormalMatrix(m_entities->at(it->second)->GetTransform()->GetModel(), m_entities->at(it->second)->GetTransform()->GetNormal());
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

//#if defined(_DEBUG) || defined(DEBUG)
// 	m_Game_renderPasses.back()->QueueRenderable(&wpTestLine);
//#endif //Line debug

	for (u32 i = 0; i < m_Game_renderPasses.size(); i++)
	{
		m_Game_renderPasses[i]->Render();
	}

	//Swap Buffers
	glfwSwapBuffers(m_window);
}