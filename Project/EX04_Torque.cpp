#include "stdafx.h"
#include "EX04_Torque.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

enum class InputIds
{
	Up,
	Down,
	Left,
	Right
};

void EX04_Torque::Initialize()
{
	EnablePhysxDebugRendering(true);

	// Get physics engine
	const auto pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	// Create default physics data
	const PxMaterial* pDefaultMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.2f) };

	// Cube data
	constexpr XMFLOAT3 actorDimensions{ 1.0f, 1.0f, 1.0f };

	// Create cube visuals
	auto pCube{ new CubePosColorNorm{ actorDimensions.x, actorDimensions.y, actorDimensions.z } };
	AddGameObject(pCube);

	// Create cube physics actor
	m_pRb = pPhysics->createRigidDynamic(PxTransform{ PxIdentity });
	const PxBoxGeometry cubeGeometry{ actorDimensions.x / 2.0f, actorDimensions.y / 2.0f, actorDimensions.z / 2.0f };

	PxRigidActorExt::createExclusiveShape(*m_pRb, cubeGeometry, *pDefaultMaterial);

	// Link cube with the cube actor
	pCube->AttachRigidActor(m_pRb);

	// Transform the cube
	pCube->Translate(0.0f, 0.5f, 0.0f);

	// Create a infinite groundplane
	const auto pGroundActor{ pPhysics->createRigidStatic(PxTransform{ PxQuat{ PxPiDivTwo, PxVec3{ 0.0f, 0.0f, 1.0f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Input setup
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Down), InputTriggerState::down, VK_DOWN
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Up), InputTriggerState::down, VK_UP
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Right), InputTriggerState::down, VK_RIGHT
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Left), InputTriggerState::down, VK_LEFT
		});
}

void EX04_Torque::Update()
{
	const float force{ 3.0f };

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right))) m_pRb->addTorque(PxVec3{ 0.0f, 0.0f, -force });
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left))) m_pRb->addTorque(PxVec3{ 0.0f, 0.0f, force });
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up))) m_pRb->addTorque(PxVec3{ force, 0.0f, 0.0f });
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down))) m_pRb->addTorque(PxVec3{ -force, 0.0f, 0.0f });
}

void EX04_Torque::Draw() const
{
}

void EX04_Torque::OnSceneActivated()
{
}

void EX04_Torque::OnSceneDeactivated()
{
}
