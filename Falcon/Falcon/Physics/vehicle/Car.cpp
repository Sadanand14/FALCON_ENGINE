#include "Car.h"
#include "Core/Components/TransformComponent.h"
#include "Rendering/PipeLine/Mesh.h"
#include "../PXMathUtils.h"
namespace physics
{
	namespace vehicle
	{

		namespace car_data
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
			((CHASSIS_DIMS.y* CHASSIS_DIMS.y + CHASSIS_DIMS.z * CHASSIS_DIMS.z)* CHASSIS_MASS / 12.0f,
				(CHASSIS_DIMS.x* CHASSIS_DIMS.x + CHASSIS_DIMS.z * CHASSIS_DIMS.z) * 0.8f * CHASSIS_MASS / 12.0f,
				(CHASSIS_DIMS.x* CHASSIS_DIMS.x + CHASSIS_DIMS.y * CHASSIS_DIMS.y)* CHASSIS_MASS / 12.0f);
			const physx::PxVec3 CHASSIS_CM_OFFSET(0.0f, -CHASSIS_DIMS.y * 0.5f + 0.65f, 0.25f);

			//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
			//Moment of inertia is just the moment of inertia of a cylinder.
			const float WHEEL_MASS = 20.0f;
			const float WHEEL_RADIUS = 0.5f;
			const float WHEEL_WIDTH = 0.4f;
			const float WHEEL_MOI = 0.5f * WHEEL_MASS * WHEEL_RADIUS * WHEEL_RADIUS;
			const float NUMBER_OF_WHEELS = 4;
			const physx::PxMeshScale SCALE_DOWN(physx::PxVec3(0.01, 0.01, 0.01), physx::PxQuat(0,0,0,1));

			//Temp Material

			//physx::PxMaterial* CAR_MATERIAL = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);



			/*------------------------------------------------------------------------------------------*/

			/**
			* Helper method to calculate the Center offset. Required for car, but doesn't need to be in Car class.
			*/
			void ComputeWheelCenterActorOffsets4W(const float wheelFrontZ, 
						const float wheelRearZ, const physx::PxVec3& chassisDims, const float wheelWidth, 
						const float wheelRadius, const float numWheels, physx::PxVec3* wheelCentreOffsets)
			{
				//chassisDims.z is the distance from the rear of the chassis to the front of the chassis.
				//The front has z = 0.5*chassisDims.z and the rear has z = -0.5*chassisDims.z.
				//Compute a position for the front wheel and the rear wheel along the z-axis.
				//Compute the separation between each wheel along the z-axis.
				const float numLeftWheels = numWheels / 2.0f;
				const float deltaZ = (wheelFrontZ - wheelRearZ) / (numLeftWheels - 1.0f);
				//Set the outside of the left and right wheels to be flush with the chassis.
				//Set the top of the wheel to be just touching the underside of the chassis.
				//Begin by setting the rear-left/rear-right/front-left,front-right wheels.
				wheelCentreOffsets[physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT]  = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + 0 * deltaZ * 0.5f);
				wheelCentreOffsets[physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + 0 * deltaZ * 0.5f);
				wheelCentreOffsets[physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + (numLeftWheels - 1) * deltaZ);
				wheelCentreOffsets[physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT]= physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + (numLeftWheels - 1) * deltaZ);
				//Set the remaining wheels.
				for (uint32_t i = 2, wheelCount = 4; i < numWheels - 2; i += 2, wheelCount += 2)
				{
					wheelCentreOffsets[wheelCount + 0] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i * deltaZ * 0.5f);
					wheelCentreOffsets[wheelCount + 1] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i * deltaZ * 0.5f);
				}
			}

			/**
			* Helper method to setup Wheel simulation data. 
			*/

			void SetupWheelsSimulationData
			(const float wheelMass, const float wheelMOI, const float wheelRadius, const float wheelWidth,
				const uint32_t numWheels, const physx::PxVec3* wheelCenterActorOffsets,
				const physx::PxVec3& chassisCMOffset, const float chassisMass,
				physx::PxVehicleWheelsSimData* wheelsSimData)
			{
				//Set up the wheels.
				physx::PxVehicleWheelData wheels[PX_MAX_NB_WHEELS];
				{
					//Set up the wheel data structures with mass, moi, radius, width.
					for (uint32_t i = 0; i < numWheels; i++)
					{
						wheels[i].mMass = wheelMass;
						wheels[i].mMOI = wheelMOI;
						wheels[i].mRadius = wheelRadius;
						wheels[i].mWidth = wheelWidth;
					}

					//Enable the handbrake for the rear wheels only.
					wheels[physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxHandBrakeTorque = 4000.0f;
					wheels[physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxHandBrakeTorque = 4000.0f;
					//Enable steering for the front wheels only.
					wheels[physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxSteer = physx::PxPi * 0.3333f;
					wheels[physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxSteer = physx::PxPi * 0.3333f;
				}

				//Set up the tires.
				physx::PxVehicleTireData tires[PX_MAX_NB_WHEELS];
				{
					//Set up the tires.
					for (uint32_t i = 0; i < numWheels; i++)
					{
						tires[i].mType = TIRE_TYPE_NORMAL;
					}
				}

				//Set up the suspensions
				physx::PxVehicleSuspensionData suspensions[PX_MAX_NB_WHEELS];
				{
					//Compute the mass supported by each suspension spring.
					float suspSprungMasses[PX_MAX_NB_WHEELS];
					PxVehicleComputeSprungMasses
					(numWheels, wheelCenterActorOffsets,
						chassisCMOffset, chassisMass, 1, suspSprungMasses);

					//Set the suspension data.
					for (uint32_t i = 0; i < numWheels; i++)
					{
						suspensions[i].mMaxCompression = 0.3f;
						suspensions[i].mMaxDroop = 0.1f;
						suspensions[i].mSpringStrength = 35000.0f;
						suspensions[i].mSpringDamperRate = 4500.0f;
						suspensions[i].mSprungMass = suspSprungMasses[i];
					}

					//Set the camber angles.
					const float camberAngleAtRest = 0.0;
					const float camberAngleAtMaxDroop = 0.01f;
					const float camberAngleAtMaxCompression = -0.01f;
					for (uint32_t i = 0; i < numWheels; i += 2)
					{
						suspensions[i + 0].mCamberAtRest = camberAngleAtRest;
						suspensions[i + 1].mCamberAtRest = -camberAngleAtRest;
						suspensions[i + 0].mCamberAtMaxDroop = camberAngleAtMaxDroop;
						suspensions[i + 1].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
						suspensions[i + 0].mCamberAtMaxCompression = camberAngleAtMaxCompression;
						suspensions[i + 1].mCamberAtMaxCompression = -camberAngleAtMaxCompression;
					}
				}

				//Set up the wheel geometry.
				physx::PxVec3 suspTravelDirections[PX_MAX_NB_WHEELS];
				physx::PxVec3 wheelCentreCMOffsets[PX_MAX_NB_WHEELS];
				physx::PxVec3 suspForceAppCMOffsets[PX_MAX_NB_WHEELS];
				physx::PxVec3 tireForceAppCMOffsets[PX_MAX_NB_WHEELS];
				{
					//Set the geometry data.
					for (uint32_t i = 0; i < numWheels; i++)
					{
						//Vertical suspension travel.
						suspTravelDirections[i] = physx::PxVec3(0, -1, 0);

						//Wheel center offset is offset from rigid body center of mass.
						wheelCentreCMOffsets[i] =
							wheelCenterActorOffsets[i] - chassisCMOffset;

						//Suspension force application point 0.3 metres below 
						//rigid body center of mass.
						suspForceAppCMOffsets[i] =
							physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);

						//Tire force application point 0.3 metres below 
						//rigid body center of mass.
						tireForceAppCMOffsets[i] =
							physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
					}
				}

				//Set up the filter data of the raycast that will be issued by each suspension.
				physx::PxFilterData qryFilterData;
				setupNonDrivableSurface(qryFilterData);

				//Set the wheel, tire and suspension data.
				//Set the geometry data.
				//Set the query filter data
				for (uint32_t i = 0; i < numWheels; i++)
				{
					wheelsSimData->setWheelData(i, wheels[i]);
					wheelsSimData->setTireData(i, tires[i]);
					wheelsSimData->setSuspensionData(i, suspensions[i]);
					wheelsSimData->setSuspTravelDirection(i, suspTravelDirections[i]);
					wheelsSimData->setWheelCentreOffset(i, wheelCentreCMOffsets[i]);
					wheelsSimData->setSuspForceAppPointOffset(i, suspForceAppCMOffsets[i]);
					wheelsSimData->setTireForceAppPointOffset(i, tireForceAppCMOffsets[i]);
					wheelsSimData->setSceneQueryFilterData(i, qryFilterData);
					wheelsSimData->setWheelShapeMapping(i, int32_t(i));
				}

				//Add a front and rear anti-roll bar
				physx::PxVehicleAntiRollBarData barFront;
				barFront.mWheel0 = physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT;
				barFront.mWheel1 = physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT;
				barFront.mStiffness = 10000.0f;
				wheelsSimData->addAntiRollBarData(barFront);
				physx::PxVehicleAntiRollBarData barRear;
				barRear.mWheel0 = physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT;
				barRear.mWheel1 = physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT;
				barRear.mStiffness = 10000.0f;
				wheelsSimData->addAntiRollBarData(barRear);
			}

		}



		Car::Car(Mesh* chassiMesh, Transform chassisTransform, Mesh* wheelMesh, Transform* wheelTransforms):
			m_car(nullptr),
			m_isVehicleInAir(false),
			m_chassisMesh(chassiMesh),
			m_wheelMesh(wheelMesh)
		{
			CreateVehicleDescriptionObject();
			CreateVehicle4W(chassisTransform, wheelTransforms);
			//Register car to the global car data
			gAllCars.emplace_back(this);
		}

		void Car::CreateVehicleDescriptionObject()
		{
			//Currently using a default. May be data can be read in via json.
			m_carDesc.chassisMass = car_data::CHASSIS_MASS;
			m_carDesc.chassisDims = car_data::CHASSIS_DIMS;
			m_carDesc.chassisMOI = car_data::CHASSIS_MOI;
			m_carDesc.chassisCMOffset = car_data::CHASSIS_CM_OFFSET;
			m_carDesc.chassisMaterial = GetDefaultMaterial();
			m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);
		
			m_carDesc.wheelMass = car_data::WHEEL_MASS;
			m_carDesc.wheelRadius = car_data::WHEEL_RADIUS;
			m_carDesc.wheelWidth = car_data::WHEEL_WIDTH;
			m_carDesc.wheelMOI = car_data::WHEEL_MOI;
			m_carDesc.numWheels = car_data::NUMBER_OF_WHEELS;
			m_carDesc.wheelMaterial = GetDefaultMaterial();
			m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);
		}


		void Car::CreateVehicle4W(Transform chassisTransform, Transform* wheelTransforms)
		{
			try
			{
				const physx::PxVec3 chassisDims = m_carDesc.chassisDims;
				const float wheelWidth = m_carDesc.wheelWidth;
				const float wheelRadius = m_carDesc.wheelRadius;
				const float numWheels = m_carDesc.numWheels;

				const physx::PxFilterData& chassisSimFilterData = m_carDesc.chassisSimFilterData;
				const physx::PxFilterData& wheelSimFilterData = m_carDesc.wheelSimFilterData;

				//GetCarMesh and create collider for that. 
				CreateVehicleActor(GetDefaultMaterial(), wheelTransforms, GetDefaultMaterial(), chassisTransform);

				//Set up the sim data for the wheels.
				physx::PxVehicleWheelsSimData* wheelsSimData = physx::PxVehicleWheelsSimData::allocate(numWheels);
				{
					//Compute the wheel center offsets from the origin.
					physx::PxVec3 wheelCenterActorOffsets[PX_MAX_NB_WHEELS];
					const float frontZ = chassisDims.z * 0.3f;
					const float rearZ = -chassisDims.z * 0.3f;
					car_data::ComputeWheelCenterActorOffsets4W(frontZ, rearZ, chassisDims, wheelWidth, wheelRadius, numWheels, wheelCenterActorOffsets);

					//Set up the simulation data for all wheels.
					car_data::SetupWheelsSimulationData
					(m_carDesc.wheelMass, m_carDesc.wheelMOI, wheelRadius, wheelWidth,
						numWheels, wheelCenterActorOffsets,
						m_carDesc.chassisCMOffset, m_carDesc.chassisMass,
						wheelsSimData);
				}



				//Set up the sim data for the vehicle drive model.
				physx::PxVehicleDriveSimData4W driveSimData;
				{
					//Diff
					physx::PxVehicleDifferential4WData diff;
					diff.mType = physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
					driveSimData.setDiffData(diff);

					//Engine
					physx::PxVehicleEngineData engine;
					engine.mPeakTorque = 500.0f;
					engine.mMaxOmega = 600.0f;//approx 6000 rpm
					driveSimData.setEngineData(engine);

					//Gears
					physx::PxVehicleGearsData gears;
					gears.mSwitchTime = 0.5f;
					driveSimData.setGearsData(gears);

					//Clutch
					physx::PxVehicleClutchData clutch;
					clutch.mStrength = 10.0f;
					driveSimData.setClutchData(clutch);

					//Ackermann steer accuracy
					physx::PxVehicleAckermannGeometryData ackermann;
					ackermann.mAccuracy = 1.0f;
					ackermann.mAxleSeparation =
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).z -
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT).z;
					ackermann.mFrontWidth =
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT).x -
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).x;
					ackermann.mRearWidth =
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT).x -
						wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT).x;
					driveSimData.setAckermannGeometryData(ackermann);
				}
				//Create a vehicle from the wheels and drive sim data.
				m_car = physx::PxVehicleDrive4W::allocate(numWheels);
				m_car->setup(GetPhysics(), nullptr/*veh4WActor need to create*/, *wheelsSimData, driveSimData, numWheels - 4);

				//Configure the userdata
				ConfigureCarData(m_car, m_carDesc.actorUserData, m_carDesc.shapeUserDatas);

				//Free the sim data because we don't need that any more.
				wheelsSimData->free();
				
			}
			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to create PxVehicle4W. {0}", e.what());
				return;
			}
			
		}



		physx::PxRigidDynamic* Car::CreateVehicleActor(physx::PxMaterial* wheelMaterials, Transform* wheelTransforms,
													   physx::PxMaterial* chassisMaterials, Transform& chassisTransform,
													   const float numChassisMeshes/*=1*/)
		{

			try {
				physx::PxPhysics* physics = GetPhysics();

				//We need a rigid body actor for the vehicle.
				//Don't forget to add the actor to the scene after setting up the associated vehicle.

				physx::PxRigidDynamic* vehActor = physics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));

				//Wheel and chassis query filter data.
				//Optional: cars don't drive on other cars.
				physx::PxFilterData wheelQryFilterData;
				setupNonDrivableSurface(wheelQryFilterData);

				physx::PxFilterData chassisQryFilterData;
				setupNonDrivableSurface(chassisQryFilterData);

				//GetMeshcollider shapes for the wheels and the chassis
				std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> wheeltemp;
				std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> chassistemp;
				m_wheelMesh->GetVertexPositionsArray(wheeltemp);
				physx::PxConvexMesh* wheelConvexMesh = GetConvexMesh(&wheeltemp[0], wheeltemp.size(), sizeof(glm::vec3));
				m_chassisMesh->GetVertexPositionsArray(chassistemp);
				physx::PxConvexMesh* chassisConvexMesh = GetConvexMesh(&chassistemp[0], chassistemp.size(), sizeof(glm::vec3));

				physx::PxVec3 *localpos;
				physx::PxQuat *localrot;
				//Add all the wheel shapes to the actor.
				for (uint8_t i = 0; i < m_carDesc.numWheels; i++)
				{
					physx::PxConvexMeshGeometry geom(wheelConvexMesh, car_data::SCALE_DOWN);
					physx::PxShape* wheelShape = physx::PxRigidActorExt::createExclusiveShape(*vehActor, geom, *wheelMaterials);
					wheelShape->setQueryFilterData(wheelQryFilterData);
					wheelShape->setSimulationFilterData(m_carDesc.wheelSimFilterData);
					/*
						Wheel transforms are needed here.
					*/
					localpos = PXMathUtils::Vec3ToPxVec3(wheelTransforms[i].GetPosition());
					localrot = PXMathUtils::QuatToPxQuat(wheelTransforms[i].GetRotation());
					wheelShape->setLocalPose(physx::PxTransform(*localpos,*localrot));
				}

				//Add the chassis shapes to the actor.
				for (uint8_t i = 0; i < numChassisMeshes; i++)
				{
					physx::PxShape* chassisShape = physx::PxRigidActorExt::createExclusiveShape(*vehActor, physx::PxConvexMeshGeometry(chassisConvexMesh, car_data::SCALE_DOWN), *chassisMaterials);
					chassisShape->setQueryFilterData(chassisQryFilterData);
					chassisShape->setSimulationFilterData(m_carDesc.chassisSimFilterData);
					/*
						Chassis Transforms are needed here
					*/
					localpos = PXMathUtils::Vec3ToPxVec3(chassisTransform.GetPosition());
					localrot = PXMathUtils::QuatToPxQuat(chassisTransform.GetRotation());
					chassisShape->setLocalPose(physx::PxTransform(*localpos, *localrot));
				}

				vehActor->setMass(m_carDesc.chassisMass);
				vehActor->setMassSpaceInertiaTensor(m_carDesc.chassisMOI);
				vehActor->setCMassLocalPose(physx::PxTransform(m_carDesc.chassisCMOffset, physx::PxQuat(physx::PxIdentity)));

				return vehActor;
			}

			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR:Failed to create Car actor. {0}", e.what());
				return nullptr;
			}
		}
		
		
		
		void Car::ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas)
		{

			if (actorUserData)
			{
				vehicle->getRigidDynamicActor()->userData = actorUserData;
				actorUserData->vehicle = vehicle;
			}

			if (shapeUserDatas)
			{
				physx::PxShape* shapes[PX_MAX_NB_WHEELS + 1];
				vehicle->getRigidDynamicActor()->getShapes(shapes, PX_MAX_NB_WHEELS + 1);
				for (uint32_t i = 0; i < vehicle->mWheelsSimData.getNbWheels(); i++)
				{
					const uint32_t shapeId = vehicle->mWheelsSimData.getWheelShapeMapping(i);
					shapes[shapeId]->userData = &shapeUserDatas[i];
					shapeUserDatas[i].isWheel = true;
					shapeUserDatas[i].wheelId = i;
				}
			}

		}
	}
}