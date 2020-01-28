#include "Car.h"

namespace physics
{
	namespace vehicle
	{

		namespace 
		{
			/**
			* Some default values for the car creation. Not for final game.
			* Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
			* The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
			* Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
			*/
			const float CHASSIS_MASS = 1500.0f;
			const physx::PxVec3 CHASSIS_DIMS(2.5f, 2.0f, 5.0f);
			const physx::PxVec3 CHASSIS_MOI
			((CHASSIS_DIMS.y* CHASSIS_DIMS.y + CHASSIS_DIMS.z * CHASSIS_DIMS.z)* chassisMass / 12.0f,
				(CHASSIS_DIMS.x* CHASSIS_DIMS.x + CHASSIS_DIMS.z * CHASSIS_DIMS.z) * 0.8f * chassisMass / 12.0f,
				(CHASSIS_DIMS.x* CHASSIS_DIMS.x + CHASSIS_DIMS.y * CHASSIS_DIMS.y)* chassisMass / 12.0f);
			const physx::PxVec3 CHASSIS_CM_OFFSET(0.0f, -CHASSIS_DIMS.y * 0.5f + 0.65f, 0.25f);

			//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
			//Moment of inertia is just the moment of inertia of a cylinder.
			const float WHEEL_MASS = 20.0f;
			const float WHEEL_RADIUS = 0.5f;
			const float WHEEL_WIDTH = 0.4f;
			const float WHEEL_MOI = 0.5f * wheelMass * wheelRadius * wheelRadius;
			const float NUMBER_OF_WHEELS = 4;


			//Temp Material

			physx::PxMaterial* CAR_MATERIAL = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);
		}



		Car::Car():
			m_car(nullptr)
		{

		}

		void Car::CreateVehicleDescriptionObject()
		{
			m_carDesc.chassisMass = CHASSIS_MASS;
			m_carDesc.chassisDims = CHASSIS_DIMS;
			m_carDesc.chassisMOI = CHASSIS_MOI;
			m_carDesc.chassisCMOffset = CHASSIS_CM_OFFSET;
			m_carDesc.chassisMaterial = CAR_MATERIAL;
			m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);
		
			m_carDesc.wheelMass = WHEEL_MASS;
			m_carDesc.wheelRadius = WHEEL_RADIUS;
			m_carDesc.wheelWidth = WHEEL_WIDTH;
			m_carDesc.wheelMOI = WHEEL_MOI;
			m_carDesc.numWheels = NUMBER_OF_WHEELS;
			m_carDesc.wheelMaterial = CAR_MATERIAL;
			m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);
		}
		physx::PxVehicleDrive4W* Car::createVehicle4W(const VehicleDesc& vehDesc, physx::PxPhysics* physics, physx::PxCooking* cooking)
		{
			return nullptr;
		}
		physx::PxRigidDynamic* Car::createVehicleActor(const physx::PxVehicleChassisData& chassisData, physx::PxMaterial** wheelMaterials, physx::PxConvexMesh** wheelConvexMeshes, const float numWheels, const physx::PxFilterData& wheelSimFilterData, physx::PxMaterial** chassisMaterials, physx::PxConvexMesh** chassisConvexMeshes, const float numChassisMeshes, const physx::PxFilterData& chassisSimFilterData, physx::PxPhysics& physics)
		{
			return nullptr;
		}
		void Car::ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas)
		{
		}
	}
}