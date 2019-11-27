
#include "Physics.h"
#include "PXMathUtils.h"
#include "PhysicsSystem.h"

#define PVD_HOST "127.0.0.1"

namespace physics
{

	/* PhysX default variables*/

	static physx::PxDefaultAllocator		gAllocator;
	static physx::PxDefaultErrorCallback	gErrorCallback;

	static physx::PxFoundation* gFoundation = NULL;
	static physx::PxPhysics* gPhysics = NULL;

	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	physx::PxScene* gScene = NULL;

	physx::PxMaterial* gMaterial = NULL;

	physx::PxPvd* gPvd = NULL;

	bool InitPhysX()
	{
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

		if (!gFoundation)
		{
			FL_ENGINE_ERROR("PX foundation cration failed");
			return false;
		}

		

		/* Uncomment this part if you want to connect the program to the PVD*/
		gPvd = PxCreatePvd(*gFoundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);

		if (!gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::ePROFILE))
		{
			FL_ENGINE_ERROR("ERROR:Failed to connect to pvd");
		}
		else 
		{
			FL_ENGINE_INFO("Info: Connected to pvd. {0}", gPvd->isConnected());
		}

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
		CreatePhysicsScene();

		physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}


		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
		//CreatePlane();
		return true;
	}

	void StepPhysics(float& dt, 
					 boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity,
					 const size_t& count)
	{
		gScene->simulate(1.0f / 60.0f);
		gScene->fetchResults(true);

		//Update physics System;
		PhysicsSystem::update(dt, entity,count);
	}

	void CreatePhysicsScene()
	{
		physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);
	}

	bool ShutdownPhysX()
	{
		try {
			FL_ENGINE_INFO("INFO: Releasing physx resources.");
			PX_RELEASE(gScene);
			PX_RELEASE(gDispatcher);
			PX_RELEASE(gPhysics);
			if (gPvd)
			{
				physx::PxPvdTransport* transport = gPvd->getTransport();
				gPvd->release();
				gPvd = NULL;
				PX_RELEASE(transport);
			}
			PX_RELEASE(gFoundation);
			return true;
		}
		catch (std::exception & e)
		{
			FL_ENGINE_ERROR("ERROR: Can't release physx.{0}", e.what());
			return false;
		}
	}

	physx::PxRigidStatic* CreatePlane()
	{
		physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 10), *gMaterial);
		gScene->addActor(*groundPlane);
		return groundPlane;
	}

	physx::PxRigidStatic* CreateStaticRigidActor(const Transform* transform, physx::PxShape* collider)
	{
		//physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(2.0f, 2.0f, 2.0f), *gMaterial);
		physx::PxVec3* localpos = PXMathUtils::Vec3ToPxVec3(transform->GetPosition());
		physx::PxQuat* localrot = PXMathUtils::QuatToPxQuat(transform->GetRotation());
		physx::PxTransform localTm(*localpos, *localrot);

		physx::PxRigidStatic* body = physx::PxCreateStatic(*gPhysics, localTm, *collider);

		if (body)
		{
			gScene->addActor(*body);
			return body;
		}
		else
		{
			FL_ENGINE_ERROR("ERROR: Failed to create actor. {0}, {1}", __FUNCTION__, __LINE__);
			return nullptr;
		}
	}

	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform, physx::PxShape* collider)
	{
		//physx::PxShape* shape = gPhysics->createShape(physx::PxSphereGeometry(2.0f), *gMaterial);
		physx::PxVec3* localpos = PXMathUtils::Vec3ToPxVec3(transform->GetPosition());
		physx::PxQuat* localrot = PXMathUtils::QuatToPxQuat(transform->GetRotation());
		physx::PxTransform localTm(*localpos, *localrot);

		physx::PxRigidDynamic* body = physx::PxCreateDynamic(*gPhysics, localTm, *collider, 10.0f);
			//physx::PxCreateDynamic(*gPhysics, localTm, physx::PxSphereGeometry(5), *gMaterial, 10.0f);
		
		//PxCreateDynamic(*gPhysics, localTm, physx::PxSphereGeometry(5), *gMaterial, 10.0f);
		if (body)
		{
			/*body->setMass(1.0f);
			physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);*/
			gScene->addActor(*body);
			return body;
		}
		else
		{
			FL_ENGINE_ERROR("ERROR: Failed to create actor. {0}, {1}", __FUNCTION__, __LINE__);
			return nullptr;
		}

	}


	physx::PxShape* GetBoxCollider(const float& halfX, const float& halfY, const float& halfZ)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfX, halfY, halfZ), *gMaterial);
		return shape;
	}
	
	physx::PxShape* GetSphereCollider(const float& radius)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxSphereGeometry(radius), *gMaterial);
		return shape;
	}

}