#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
#include "VehicleFilterShader.h"
#include "VehicleSceneQuery.h"

#define WHEEL_COUNT 4

namespace physics
{
	namespace vehicle
	{
		

		struct ActorUserData
		{
			ActorUserData()
				: vehicle(NULL),
				actor(NULL)
			{
			}

			const physx::PxVehicleWheels* vehicle;
			const physx::PxActor* actor;
		};

		struct ShapeUserData
		{
			ShapeUserData()
				: isWheel(false),
				wheelId(0xffffffff)
			{
			}

			bool isWheel;
			uint32_t wheelId;
		};


		struct VehicleDesc
		{
			float chassisMass;
			physx::PxVec3 chassisDims;
			physx::PxVec3 chassisMOI;
			physx::PxVec3 chassisCMOffset;
			physx::PxMaterial* chassisMaterial;
			physx::PxFilterData chassisSimFilterData;  //word0 = collide type, word1 = collide against types, word2 = physx::PxPairFlags

			float wheelMass;
			float wheelWidth;
			float wheelRadius;
			float wheelMOI;
			physx::PxMaterial* wheelMaterial;
			float numWheels;
			physx::PxFilterData wheelSimFilterData;	//word0 = collide type, word1 = collide against types, word2 = physx::PxPairFlags

			ActorUserData* actorUserData;
			ShapeUserData* shapeUserDatas;


			VehicleDesc()
				: chassisMass(0.0f),
				chassisDims(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisMOI(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisCMOffset(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisMaterial(NULL),
				wheelMass(0.0f),
				wheelWidth(0.0f),
				wheelRadius(0.0f),
				wheelMOI(0.0f),
				wheelMaterial(NULL),
				actorUserData(NULL),
				shapeUserDatas(NULL)
			{
			}
		};


		/**
		* The API which exposes the vehicle creation for the external classes to create classes
		* to create and use cars in scenes.
		* Steps to instantiate a car 
		* 1. Setup wheel data
		* 2. Setup DriveSimulation data
		* 3. Setup Colliders and shapes
		* 4. Add to scene
		*/
		class Car
		{
			
		private:
			VehicleDesc m_carDesc;
			physx::PxVehicleDrive4W* m_car;
			void CreateVehicleDescriptionObject();

			physx::PxVehicleDrive4W* createVehicle4W (const VehicleDesc& vehDesc, physx::PxPhysics* physics, physx::PxCooking* cooking);

			physx::PxRigidDynamic* createVehicleActor 
				(const physx::PxVehicleChassisData& chassisData,
					physx::PxMaterial** wheelMaterials, physx::PxConvexMesh** wheelConvexMeshes, const float numWheels, const physx::PxFilterData& wheelSimFilterData,
					physx::PxMaterial** chassisMaterials, physx::PxConvexMesh** chassisConvexMeshes, const float numChassisMeshes, const physx::PxFilterData& chassisSimFilterData,
					physx::PxPhysics& physics);

			void ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas);
		public:
			Car();
		};

	}
}

#endif // !CAR_H
