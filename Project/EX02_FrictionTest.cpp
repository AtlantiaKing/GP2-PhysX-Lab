#include "stdafx.h"
#include "EX02_FrictionTest.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void EX02_FrictionTest::Initialize()
{
	EnablePhysxDebugRendering(true);

	// Get physics engine
	const auto pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	// Create physics materials
	const PxMaterial* pNoFricMaterial{ pPhysics->createMaterial(0.0f, 0.0f, 0.0f) };
	const PxMaterial* pHalfFricMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.0f) };
	const PxMaterial* pFullFricMaterial{ pPhysics->createMaterial(1.0f, 1.0f, 0.0f) };

	// Ramp data
	constexpr XMFLOAT3 rampDimensions{ 5.0f, 1.0f, 1.0f };
	constexpr float gapBetweenRamps{ 7.0f };

	// Create 3 ramps
	for (int i{}; i < 3; ++i)
	{
		// Create cube visuals
		auto pRamp{ new CubePosColorNorm{rampDimensions.x, rampDimensions.y, rampDimensions.z} };
		AddGameObject(pRamp);

		// Create cube physics actor
		PxRigidStatic* pRampActor{ pPhysics->createRigidStatic(PxTransform{ PxIdentity }) };
		const PxBoxGeometry cubeGeometry{ rampDimensions.x / 2.0f, rampDimensions.y / 2.0f, rampDimensions.z / 2.0f };

		PxRigidActorExt::createExclusiveShape(*pRampActor, cubeGeometry, *pNoFricMaterial);

		// Link cube with the cube actor
		pRamp->AttachRigidActor(pRampActor);

		// Transform the cube
		pRamp->Translate(-gapBetweenRamps + i * gapBetweenRamps, 3.0f, 0.0f);
		pRamp->RotateDegrees(0.0f, 0.0f, -30);
	}

	constexpr XMFLOAT3 cubeDimensions{ 1.0f, 1.0f, 1.0f };

	// Create sphere visuals
	const auto pCube0{ new CubePosColorNorm{ cubeDimensions.x, cubeDimensions.y, cubeDimensions.z } };
	AddGameObject(pCube0);
	const auto pCube1{ new CubePosColorNorm{ cubeDimensions.x, cubeDimensions.y, cubeDimensions.z } };
	AddGameObject(pCube1);
	const auto pCube2{ new CubePosColorNorm{ cubeDimensions.x, cubeDimensions.y, cubeDimensions.z } };
	AddGameObject(pCube2);

	// Create sphere physics actor
	PxRigidDynamic* pBoxActor0{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidDynamic* pBoxActor1{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidDynamic* pBoxActor2{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	const PxBoxGeometry cubeGeo{ PxBoxGeometry{ cubeDimensions.x / 2.0f, cubeDimensions.y / 2.0f, cubeDimensions.z / 2.0f } };

	PxRigidActorExt::createExclusiveShape(*pBoxActor0, cubeGeo, *pNoFricMaterial);
	PxRigidActorExt::createExclusiveShape(*pBoxActor1, cubeGeo, *pHalfFricMaterial);
	PxRigidActorExt::createExclusiveShape(*pBoxActor2, cubeGeo, *pFullFricMaterial);

	// Link sphere with the sphere actor
	pCube0->AttachRigidActor(pBoxActor0);
	pCube1->AttachRigidActor(pBoxActor1);
	pCube2->AttachRigidActor(pBoxActor2);

	// Transform the sphered
	pCube0->Translate(-gapBetweenRamps - rampDimensions.x / 4.0f, 5.0f, 0.0f);
	pCube0->RotateDegrees(0.0f, 0.0f, -30.0f);
	pCube1->Translate(-rampDimensions.x / 4.0f, 5.0f, 0.0f);
	pCube1->RotateDegrees(0.0f, 0.0f, -30.0f);
	pCube2->Translate(gapBetweenRamps - rampDimensions.x / 4.0f, 5.0f, 0.0f);
	pCube2->RotateDegrees(0.0f, 0.0f, -30.0f);

	// Create a infinite groundplane
	const auto pGroundActor{ pPhysics->createRigidStatic(PxTransform{ PxQuat{ PxPiDivTwo, PxVec3{ 0.0f, 0.0f, 1.0f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pHalfFricMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void EX02_FrictionTest::Update()
{
}

void EX02_FrictionTest::Draw() const
{
}

void EX02_FrictionTest::OnSceneActivated()
{
}

void EX02_FrictionTest::OnSceneDeactivated()
{
}
