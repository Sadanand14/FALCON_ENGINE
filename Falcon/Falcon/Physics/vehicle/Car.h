#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
#include "VehicleFilterShader.h"
#include "VehicleSceneQuery.h"
#define WHEEL_COUNT 4

class Mesh;
struct Transform;

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
			bool m_isVehicleInAir;
			Mesh* m_chassisMesh;
			Mesh* m_wheelMesh;


			void CreateVehicleDescriptionObject();

			void CreateVehicle4W(Transform chassisTransform, Transform* wheelTransforms);

			physx::PxRigidDynamic* CreateVehicleActor( physx::PxMaterial* wheelMaterials, Transform* wheelTransforms,
													   physx::PxMaterial* chassisMaterials, Transform& chassisTransform,
													   const float numChassisMeshes = 1);
			void ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas);
		public:
			Car(Mesh* chassiMesh, Transform chassisTransform,Mesh* wheelMesh, Transform* wheelTransforms);

			//void Update(float dt);

			~Car()
			{
				PX_RELEASE(m_car);
				//m_car->free();

			}
			

			inline physx::PxVehicleDrive4W* GetDriveComponent() const { return m_car; }
			inline void SetIsInAir(bool val) { m_isVehicleInAir = val; }
		};

	}
}

#endif // !CAR_H
