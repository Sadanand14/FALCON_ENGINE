#include "CarAPI.h"
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

		/**
		* Nvidia'a Sample code to itereate through multiple driving routines. Being used here for testing.
		* Will be removed once we have inputs coming in via Falcon's APIs.
		*
		*/
		void IncrementDrivingMode(Car* car,const float timestep)
		{
			gVehicleModeTimer += timestep;
			if (gVehicleModeTimer > gVehicleModeLifetime)
			{
				//If the mode just completed was eDRIVE_MODE_ACCEL_REVERSE then switch back to forward gears.
				if (eDRIVE_MODE_ACCEL_REVERSE == gDriveModeOrder[gVehicleOrderProgress])
				{
					car->m_car->mDriveDynData.forceGearChange(physx::PxVehicleGearsData::eFIRST);
				}

				//Increment to next driving mode.
				gVehicleModeTimer = 0.0f;
				gVehicleOrderProgress++;
				releaseAllControls(car);

				//If we are at the end of the list of driving modes then start again.
				if (eDRIVE_MODE_NONE == gDriveModeOrder[gVehicleOrderProgress])
				{
					gVehicleOrderProgress = 0;
					gVehicleOrderComplete = true;
				}

				//Start driving in the selected mode.
				DriveMode eDriveMode = gDriveModeOrder[gVehicleOrderProgress];
				switch (eDriveMode)
				{
				case eDRIVE_MODE_ACCEL_FORWARDS:
					startAccelerateForwardsMode(car);
					break;
				case eDRIVE_MODE_ACCEL_REVERSE:
					startAccelerateReverseMode(car);
					break;
				case eDRIVE_MODE_HARD_TURN_LEFT:
					startTurnHardLeftMode(car);
					break;
				case eDRIVE_MODE_HANDBRAKE_TURN_LEFT:
					startHandbrakeTurnLeftMode(car);
					break;
				case eDRIVE_MODE_HARD_TURN_RIGHT:
					startTurnHardRightMode(car);
					break;
				case eDRIVE_MODE_HANDBRAKE_TURN_RIGHT:
					startHandbrakeTurnRightMode(car);
					break;
				case eDRIVE_MODE_BRAKE:
					startBrakeMode(car);
					break;
				case eDRIVE_MODE_NONE:
					break;
				};

				//If the mode about to start is eDRIVE_MODE_ACCEL_REVERSE then switch to reverse gears.
				if (eDRIVE_MODE_ACCEL_REVERSE == gDriveModeOrder[gVehicleOrderProgress])
				{
					car->m_car->mDriveDynData.forceGearChange(physx::PxVehicleGearsData::eREVERSE);
				}
			}
		}


		void CreateVehicleDescriptionObject(Car* car)
		{
			//Currently using a default. May be data can be read in via json.
			car->m_carDesc.chassisMass = car_data::CHASSIS_MASS;
			car->m_carDesc.chassisDims = car_data::CHASSIS_DIMS;
			car->m_carDesc.chassisMOI = car_data::CHASSIS_MOI;
			car->m_carDesc.chassisCMOffset = car_data::CHASSIS_CM_OFFSET;
			car->m_carDesc.chassisMaterial = GetDefaultMaterial();
			car->m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);
		
			car->m_carDesc.wheelMass = car_data::WHEEL_MASS;
			car->m_carDesc.wheelRadius = car_data::WHEEL_RADIUS;
			car->m_carDesc.wheelWidth = car_data::WHEEL_WIDTH;
			car->m_carDesc.wheelMOI = car_data::WHEEL_MOI;
			car->m_carDesc.numWheels = car_data::NUMBER_OF_WHEELS;
			car->m_carDesc.wheelMaterial = GetDefaultMaterial();
			car->m_carDesc.chassisSimFilterData = physx::PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);
		}


		void CreateVehicle4W(Car* car,physx::PxRigidDynamic* vehActor)
		{
			try
			{
				const physx::PxVec3 chassisDims = car->m_carDesc.chassisDims;
				const float wheelWidth = car->m_carDesc.wheelWidth;
				const float wheelRadius = car->m_carDesc.wheelRadius;
				const float numWheels = car->m_carDesc.numWheels;

				const physx::PxFilterData& chassisSimFilterData = car->m_carDesc.chassisSimFilterData;
				const physx::PxFilterData& wheelSimFilterData = car->m_carDesc.wheelSimFilterData;

				//GetCarMesh and create collider for that. 
				SetupVehicleActorData(car,vehActor);
				//GetPhysicsScene()->addActor(*vehActor);
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
					(car->m_carDesc.wheelMass, car->m_carDesc.wheelMOI, wheelRadius, wheelWidth,
						numWheels, wheelCenterActorOffsets,
						car->m_carDesc.chassisCMOffset, car->m_carDesc.chassisMass,
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
				car->m_car = physx::PxVehicleDrive4W::allocate(numWheels);
				car->m_car->setup(GetPhysics(), vehActor/*veh4WActor need to create*/, *wheelsSimData, driveSimData, numWheels - 4);


				//Set the vehicle to rest in first gear.
				//Set the vehicle to use auto-gears.
				car->m_car->setToRestState();
				car->m_car->mDriveDynData.forceGearChange(physx::PxVehicleGearsData::eFIRST);
				car->m_car->mDriveDynData.setUseAutoGears(true);

				startBrakeMode(car);

				//Configure the userdata
				ConfigureCarData(car->m_car, car->m_carDesc.actorUserData, car->m_carDesc.shapeUserDatas);

				//Free the sim data because we don't need that any more.
				wheelsSimData->free();
				
			}
			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to create PxVehicle4W. {0}", e.what());
				return;
			}
			
		}



		void SetupVehicleActorData(Car* car, physx::PxRigidDynamic* vehActor)
		{

			try {

				//Wheel and chassis query filter data.
				//Optional: cars don't drive on other cars.
				physx::PxFilterData wheelQryFilterData;
				setupNonDrivableSurface(wheelQryFilterData);

				physx::PxFilterData chassisQryFilterData;
				setupNonDrivableSurface(chassisQryFilterData);


				//Setting physical properties for the car
				vehActor->setMass(car->m_carDesc.chassisMass);
				vehActor->setMassSpaceInertiaTensor(car->m_carDesc.chassisMOI);
				vehActor->setCMassLocalPose(physx::PxTransform(car->m_carDesc.chassisCMOffset, physx::PxQuat(physx::PxIdentity)));
				return;
			}

			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR:Failed to setup Car actor data. {0}", e.what());
				return ;
			}
		}
		
		
		
		void ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas)
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


		void startAccelerateForwardsMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalAccel(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogAccel(1.0f);
			}
		}

		void startAccelerateReverseMode(Car* car)
		{
			car->m_car->mDriveDynData.forceGearChange(physx::PxVehicleGearsData::eREVERSE);

			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalAccel(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogAccel(1.0f);
			}
		}

		void startBrakeMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalBrake(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogBrake(1.0f);
			}
		}

		void startTurnHardLeftMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalAccel(true);
				car->m_vehicleInputData.setDigitalSteerLeft(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogAccel(true);
				car->m_vehicleInputData.setAnalogSteer(-1.0f);
			}
		}

		void startTurnHardRightMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalAccel(true);
				car->m_vehicleInputData.setDigitalSteerRight(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogAccel(1.0f);
				car->m_vehicleInputData.setAnalogSteer(1.0f);
			}
		}

		void startHandbrakeTurnLeftMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalSteerLeft(true);
				car->m_vehicleInputData.setDigitalHandbrake(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogSteer(-1.0f);
				car->m_vehicleInputData.setAnalogHandbrake(1.0f);
			}
		}

		void startHandbrakeTurnRightMode(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalSteerRight(true);
				car->m_vehicleInputData.setDigitalHandbrake(true);
			}
			else
			{
				car->m_vehicleInputData.setAnalogSteer(1.0f);
				car->m_vehicleInputData.setAnalogHandbrake(1.0f);
			}
		}


		void releaseAllControls(Car* car)
		{
			if (gMimicKeyInputs)
			{
				car->m_vehicleInputData.setDigitalAccel(false);
				car->m_vehicleInputData.setDigitalSteerLeft(false);
				car->m_vehicleInputData.setDigitalSteerRight(false);
				car->m_vehicleInputData.setDigitalBrake(false);
				car->m_vehicleInputData.setDigitalHandbrake(false);
			}
			else
			{
				car->m_vehicleInputData.setAnalogAccel(0.0f);
				car->m_vehicleInputData.setAnalogSteer(0.0f);
				car->m_vehicleInputData.setAnalogBrake(0.0f);
				car->m_vehicleInputData.setAnalogHandbrake(0.0f);
			}
		}
	}
}