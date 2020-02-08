
#include "Physx/physx/include/vehicle/PxVehicleUtil.h"

#include "Physics.h"
#include "PXMathUtils.h"
#include "PhysicsSystem.h"
#include "PXUtils.h"
#include "vehicle/Vehicle.h"
#include "vehicle/CarAPI.h"
#define PVD_HOST "127.0.0.1"

namespace physics
{
	/*
	* Nameless namespace to maintain these functions and variables as private to physics.
	*
	*/
	namespace {
		/* PhysX default variables*/

		static physx::PxDefaultAllocator		     gAllocator;
		static FLPxErrorCallback	                 gErrorCallback;
												     
		static physx::PxFoundation*                  gFoundation = NULL;
		static physx::PxPhysics*                     gPhysics = NULL;
		static physx::PxCooking*                     gCooking = NULL;
												     
		static physx::PxDefaultCpuDispatcher*        gDispatcher = NULL;
		static physx::PxScene*                       gScene = NULL;
												     
		static physx::PxMaterial*                    gMaterial = NULL;
												     
		static physx::PxPvd*                         gPvd = NULL;
												     
		/*Setting up variables specific to the vehicle SDK (May be this data can be moved to the vehicle namespace)*/ 
		//static physx::VehicleSceneQueryData*         gVehicleSceneQueryData = NULL;
		static physx::PxBatchQuery*                  gBatchQuery = NULL;
															      
		static physx::PxRigidStatic*				 gGroundPlane = NULL;
		static physx::PxVehicleDrive4W*				 gVehicle4W = NULL;
		static physx::PxVehicleDrivableSurfaceToTireFrictionPairs*       gFrictionPairs = NULL;
															      
		static bool					                              gIsVehicleInAir = true;

		/**
		*
		* Function creates a Random convex mesh for collider shape which is used for the mesh collider.
		* @param int number of vertecies
		* @param pointer to vertex array
		* @param int stride
		*/
		template<physx::PxConvexMeshCookingType::Enum convexMeshCookingType, bool directInsertion, int gaussMapLimit>
		physx::PxConvexMesh* createRandomConvex(int numVerts, const physx::PxVec3* verts, int stride)
		{
			physx::PxCookingParams params = gCooking->getParams();

			// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
			params.convexMeshCookingType = convexMeshCookingType;

			// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
			// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
			params.gaussMapLimit = gaussMapLimit;
			gCooking->setParams(params);

			// Setup the convex mesh descriptor
			physx::PxConvexMeshDesc desc;

			// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
			desc.points.data = verts;
			desc.points.count = numVerts;
			desc.points.stride = sizeof(physx::PxVec3);;
			//desc.quantizedCount = numVerts ;
			desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

			int meshSize = 0;
			physx::PxConvexMesh* convex = NULL;

			if (directInsertion)
			{
				// Directly insert mesh into PhysX
				convex = gCooking->createConvexMesh(desc, gPhysics->getPhysicsInsertionCallback());
				PX_ASSERT(convex);
			}
			else
			{
				// Serialize the cooked mesh into a stream.
				physx::PxDefaultMemoryOutputStream outStream;
				bool res = gCooking->cookConvexMesh(desc, outStream);
				PX_UNUSED(res);
				PX_ASSERT(res);
				meshSize = outStream.getSize();

				// Create the mesh from a stream.
				physx::PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
				convex = gPhysics->createConvexMesh(inStream);
				PX_ASSERT(convex);
			}

			return convex;
			//// Print the elapsed time for comparison
			//PxU64 stopTime = SnippetUtils::getCurrentTimeCounterValue();
			//float elapsedTime = SnippetUtils::getElapsedTimeInMilliseconds(stopTime - startTime);
			//printf("\t -----------------------------------------------\n");
			//printf("\t Create convex mesh with %d triangles: \n", numVerts);
			//directInsertion ? printf("\t\t Direct mesh insertion enabled\n") : printf("\t\t Direct mesh insertion disabled\n");
			//printf("\t\t Gauss map limit: %d \n", gaussMapLimit);
			//printf("\t\t Created hull number of vertices: %d \n", convex->getNbVertices());
			//printf("\t\t Created hull number of polygons: %d \n", convex->getNbPolygons());
			//printf("\t Elapsed time in ms: %f \n", double(elapsedTime));
			//if (!directInsertion)
			//{
			//	printf("\t Mesh size: %d \n", meshSize);
			//}

			//convex->release();
		}

		physx::PxMeshScale scaleDown(physx::PxVec3(0.01, 0.01, 0.01), physx::PxQuat(0,0,0,1));
		physx::PxRigidStatic* gGround;

	}


	physx::PxPhysics* GetPhysics() { return gPhysics; }
	physx::PxScene* GetPhysicsScene() { return gScene;	}
	physx::PxCooking* GetCooking() { return gCooking; }
	physx::PxDefaultAllocator GetAllocator() { return gAllocator; };
	physx::PxMaterial* GetDefaultMaterial() { return gMaterial; }

	/*
	* Initiates the physX system.
	* @return true is success. false on failure with error log.
	*/
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

		
		if (!gPvd->connect(*transport,  physx::PxPvdInstrumentationFlag::eDEBUG | physx::PxPvdInstrumentationFlag::eMEMORY))
		{
			FL_ENGINE_ERROR("ERROR:Failed to connect to pvd");
		}
		else
		{
			FL_ENGINE_INFO("Info: Connected to pvd. {0}", gPvd->isConnected());
		}

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
		gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
		CreatePhysicsScene();

		/*Will connect to NVIDIA PVD only in debug mode*/

#ifdef BUILD_DEBUG_MODE
		
		physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif

		//Creating default material for the generic use
		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
		if (!vehicle::InitVehicleSDK())
		{
			FL_ENGINE_ERROR("ERROR: FATAL. Failed to initialize vehicle sdk.");
		};


		CreatePlane();
		return true;
	}


	/**
	* Executes the physics simulation for dt.
	* @param dt timedelta
	* @param vector of entities with physics component on them
	* @param count of entities
	*/

	void StepPhysics(float& dt,
		boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entity,
		const size_t& count)
	{

		//Update vehicles
		vehicle::StepVehicleSDK(1.0f / 60.0f);

		gScene->simulate(1.0f / 60.0f);
		gScene->fetchResults(true);

		//Update physics System;
		PhysicsSystem::update(dt, entity, count);
	}


	/**
	* Creates the physX scene.
	*/

	void CreatePhysicsScene()
	{ 
		physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -2.0f, 0.0f);
		gDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);
	}

	/**
	* Shuts down the physX.
	* @return true is success. false on failure with error log.
	*/

	bool ShutdownPhysX()
	{
		try {

			FL_ENGINE_INFO("INFO: Releasing physx resources.");

			vehicle::ReleaseVehcileSDK();
			PX_RELEASE(gGround);
			PX_RELEASE(gScene);
			PX_RELEASE(gDispatcher);
			PX_RELEASE(gPhysics);
			PX_RELEASE(gCooking);

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

	/**
	* Creats plane at origin with normal pointing to Y+ve. Soon will be updated to take in
	* position.
	*/
	physx::PxRigidStatic* CreatePlane()
	{
		gGround = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
		gScene->addActor(*gGround);
		return gGround;
	}


	/**
	* Creates Static Rigid body.
	* @param Transform of the entity.
	* @param Collider for the body.
	* @return Returns pointer to PxRigidStatic actor.
	*/
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

	/**
	* Creates Dynamic Rigid body.
	* @param Transform of the entity.
	* @param Collider for the body.
	* @return Returns pointer to PxRigidDynamic actor nullptr on failure.
	*/
	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform, physx::PxShape* collider)
	{
		
		physx::PxVec3* localpos = PXMathUtils::Vec3ToPxVec3(transform->GetPosition());
		physx::PxQuat* localrot = PXMathUtils::QuatToPxQuat(transform->GetRotation());
		physx::PxTransform localTm(*localpos, *localrot);

		physx::PxRigidDynamic* body = physx::PxCreateDynamic(*gPhysics, localTm, *collider, 10.0f);
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

	/**
	* Creates an empty rigid dynamic body which can be used to define multiple colliders.
	*/

	physx::PxRigidDynamic* CreateDynamicRigidActor() 
	{ 
		physx::PxRigidDynamic* actor = gPhysics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
		gScene->addActor(*actor);
		return actor;
	}

	/**
	* Creates box collider.
	* @param float indicating halfX
	* @param float indicating halfY
	* @param float indicating halfZ
	*/
	physx::PxShape* GetBoxCollider(const float& halfX, const float& halfY, const float& halfZ)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfX, halfY, halfZ), *gMaterial);
		return shape;
	}

	/**
	* Creates sphere collider
	* @param radius of the sphere
	* @return PxShape * for collider shape
	*/
	physx::PxShape* GetSphereCollider(const float& radius)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxSphereGeometry(radius), *gMaterial);
		return shape;
	}

	/**
	* Creates capsule collider
	* @param radius of the top and bottom sphere
	* @param halfHeight of the capsule.
	* @return PxShape * for collider shape
	*/
	physx::PxShape* GetCapsuleCollider(const float& radius, const float& halfHeight)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxCapsuleGeometry(radius, halfHeight), *gMaterial);
		return shape;
	}



	
	


	/**
	* Createsconvext mesh for the collider
	* @param vertexData vertices of the mesh
	* @param stride the length for each vertex
	* @param directInsertion defines if mesh data should be streamed or not. Default value False.
	* @return PxConvexMesh for the 
	*/
	physx::PxConvexMesh* GetConvexMesh(const glm::vec3* vertexData, const int& stride, const int& vertCount, bool directInsert)
	{
		physx::PxConvexMesh* convexMesh = nullptr;
		std::vector<physx::PxVec3> pxvertarry;
		pxvertarry.resize(vertCount);

		for (int i = 0; i < vertCount; ++i)
		{
			PXMathUtils::Vec3ToPxVec3(vertexData[i], pxvertarry[i]);
		}
		// direct insert is false = The default convex mesh creation serializing to a stream, useful for offline cooking.
		convexMesh = createRandomConvex<physx::PxConvexMeshCookingType::eQUICKHULL, false, 32>(vertCount, &pxvertarry[0], stride);
		return convexMesh;
	}

	/**
	* Creates Mesh collider
	* @param vertexData vertices of the mesh
	* @param stride the length for each vertex
	* @param directInsertion defines if mesh data should be streamed or not. Default value False.
	* @return PxShape * for collider shape
	*/

	physx::PxShape* GetMeshCollider(const glm::vec3* vertexData, const int& stride, const int& vertCount, bool directInsert /*= false*/)
	{
		physx::PxConvexMesh* convexMesh = GetConvexMesh(vertexData, stride, vertCount, directInsert);

		physx::PxConvexMeshGeometry convexMeshGeometry(convexMesh);
		physx::PxShape* shape = gPhysics->createShape(physx::PxConvexMeshGeometry(convexMesh, scaleDown), *gMaterial);
		return shape;
	}

	physx::PxShape* GetExclusiveShape(physx::PxRigidActor* actor, const Transform* transform, const glm::vec3* vertexData, const int& count, const int& stride)
	{

		try
		{
			physx::PxConvexMesh* convexMesh = GetConvexMesh(&vertexData[0], stride, count);
			physx::PxConvexMeshGeometry geom(convexMesh, scaleDown);
			physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*actor, geom, *gMaterial);
			shape->setLocalPose(physx::PxTransform(*PXMathUtils::Vec3ToPxVec3(transform->GetPosition()), *PXMathUtils::QuatToPxQuat(transform->GetRotation())));
			return shape;
		}
		
		catch(std::exception& e)
		{
			FL_ENGINE_ERROR("ERROR:Failed to create exclusive shape {0}", e.what());
			return nullptr;
		}
	}


	void CreateCar(physx::PxRigidDynamic* vehActor)
	{
		try
		{
			vehicle::CreateCar(vehActor);
		}
		catch (std::exception & e)
		{
			FL_ENGINE_ERROR("ERROR:Failed to create car {0}", e.what());
		}
	}

	void ReleaseCollider(physx::PxRigidActor* ref)
	{
		PX_RELEASE(ref);
	}





}