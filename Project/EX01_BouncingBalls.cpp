#include "stdafx.h"
#include "EX01_BouncingBalls.h"
#include "PhysXTestScene.h"
#include "SpherePosColorNorm.h"

void EX01_BouncingBalls::Initialize()
{
	EnablePhysxDebugRendering(true);

	// Get physics engine
	const auto pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	// Create physics materials
	const PxMaterial* pBounceMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 1.0f) };
	const PxMaterial* pDefaultMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.5f) };
	const PxMaterial* pNonBounceMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.0f) };

	// Sphere data
	constexpr float actorRadius{ 1.0f };
	constexpr int actorSlices{ 50 };
	constexpr int actorStacks{ 50 };
	constexpr XMFLOAT4 actorColor{ 0.0f, 0.5f, 0.0f, 1.0f };

	// Create sphere visuals
	const auto sphere0{ new SpherePosColorNorm{ actorRadius, actorSlices, actorStacks, actorColor } };
	AddGameObject(sphere0);
	const auto sphere1{ new SpherePosColorNorm{ actorRadius, actorSlices, actorStacks, actorColor } };
	AddGameObject(sphere1);
	const auto sphere2{ new SpherePosColorNorm{ actorRadius, actorSlices, actorStacks, actorColor } };
	AddGameObject(sphere2);

	// Create sphere physics actor
	PxRigidDynamic* pSphereActor0{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidDynamic* pSphereActor1{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidDynamic* pSphereActor2{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity }) };
	const PxSphereGeometry sphereGeo{ PxSphereGeometry{ actorRadius } };

	PxRigidActorExt::createExclusiveShape(*pSphereActor0, sphereGeo, *pBounceMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor1, sphereGeo, *pDefaultMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, sphereGeo, *pNonBounceMaterial);

	// Link sphere with the sphere actor
	sphere0->AttachRigidActor(pSphereActor0);
	sphere1->AttachRigidActor(pSphereActor1);
	sphere2->AttachRigidActor(pSphereActor2);

	// Transform the sphere
	sphere0->Translate(-4.0f, 15.0f, 0.0f);
	sphere1->Translate(0.0f, 10.0f, 0.0f);
	sphere2->Translate(4.0f, 5.0f, 0.0f);

	// Create a infinite groundplane
	const auto pGroundActor{ pPhysics->createRigidStatic(PxTransform{ PxQuat{ PxPiDivTwo, PxVec3{ 0.0f, 0.0f, 1.0f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void EX01_BouncingBalls::Update()
{
}

void EX01_BouncingBalls::Draw() const
{
}

void EX01_BouncingBalls::OnSceneActivated()
{
}

void EX01_BouncingBalls::OnSceneDeactivated()
{
}
