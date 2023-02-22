#include "stdafx.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	// Get physics engine
	const auto pPhysics{ PhysxManager::GetInstance()->GetPhysics()};

	// Create default physics data
	const PxMaterial* pDefaultMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.2f) };

	// Cube data
	constexpr XMFLOAT3 actorDimensions{ 1.0f, 1.0f, 1.0f };

	// Create cube visuals
	m_pCube = new CubePosColorNorm{ actorDimensions.x, actorDimensions.y, actorDimensions.z };
	AddGameObject(m_pCube);

	// Create cube physics actor
	PxRigidDynamic* pCubeActor{ pPhysics->createRigidDynamic(PxTransform{ PxIdentity })};
	const PxBoxGeometry cubeGeometry{ actorDimensions.x / 2.0f, actorDimensions.y / 2.0f, actorDimensions.z / 2.0f };

	PxRigidActorExt::createExclusiveShape(*pCubeActor, cubeGeometry, *pDefaultMaterial);
	// OR
	//	LONG VERSION
	/*PxShape* pCubeShape{ pPhysics->createShape(cubeGeometry, *pDefaultMaterial, true) };
	pCubeActor->attachShape(*pCubeShape);
	m_pPhysxScene->addActor(*pCubeActor);*/

	// Link cube with the cube actor
	m_pCube->AttachRigidActor(pCubeActor);

	// Transform the cube
	m_pCube->Translate(0.0f, 10.0f, 0.0f);
	m_pCube->Rotate(30.0f, 45.0f, 25.0f);

	// Create a infinite groundplane
	const auto pGroundActor{ pPhysics->createRigidStatic(PxTransform{ PxQuat{ PxPiDivTwo, PxVec3{ 0.0f, 0.0f, 1.0f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube->Translate(0.0f, 10.0f, 0.0f);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
