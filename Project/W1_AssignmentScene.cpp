#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

enum class InputIds
{
	Up,
	Down,
	Left,
	Right,
	Jump,
	Reset
};

void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	// Get physics engine
	const auto pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	// Create default physics data
	const PxMaterial* pCubeMaterial{ pPhysics->createMaterial(0.5f, 0.5f, 0.8f) };
	const PxMaterial* pSphereMaterial{ pPhysics->createMaterial(1.0f, 1.0f, 0.2f) };
	const PxMaterial* pFloorMaterial{ pPhysics->createMaterial(0.8f, 0.8f, 0.5f) };

	// Mesh data
	constexpr XMFLOAT3 groundDimensions{ 100.0f, 1.0f, 100.0f };
	constexpr float sphereRadius{ 1.0f };
	constexpr int slices{ 50 };
	constexpr float cubeMass{ 1.0f };
	constexpr float sphereMass{ 25 };

	// Create sphere visuals
	m_pSphere = new SpherePosColorNorm{ sphereRadius, slices, slices, XMFLOAT4{ 1.0f, 1.0f, 0.0f, 1.0f } };
	AddGameObject(m_pSphere);

	// Create sphere physics actor
	m_pRb = pPhysics->createRigidDynamic(PxTransform{ PxIdentity });
	const PxSphereGeometry sphereGeometry{ sphereRadius };

	PxRigidActorExt::createExclusiveShape(*m_pRb, sphereGeometry, *pSphereMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*m_pRb, sphereMass);

	// Link sphere with the sphere actor
	m_pSphere->AttachRigidActor(m_pRb);

	// Transform the sphere
	m_pSphere->Translate(0.0f, 1.0f, -30.0f);

	// Create the shared shape of the cubes
	const PxBoxGeometry cubeGeometry{ m_CubeDimensions.x / 2.0f, m_CubeDimensions.y / 2.0f, m_CubeDimensions.z / 2.0f };

	// Foreach cube in the wall
	for (int x{}; x < m_NrCubesInX; ++x)
	{
		for (int y{}; y < m_NrCubesInY; ++y)
		{
			// Create cube visuals
			const auto pCube{ new CubePosColorNorm{ m_CubeDimensions.x, m_CubeDimensions.y, m_CubeDimensions.z } };
			AddGameObject(pCube);

			// Create cube physics actor
			auto pCubeActor = pPhysics->createRigidDynamic(PxTransform{ PxIdentity });
			PxRigidBodyExt::setMassAndUpdateInertia(*pCubeActor, cubeMass);
			
			PxRigidActorExt::createExclusiveShape(*pCubeActor, cubeGeometry, *pCubeMaterial);

			// Link cube with the cube actor
			pCube->AttachRigidActor(pCubeActor);

			// Calculate the current position in the wall
			const XMFLOAT3 wallPos{ m_WallStartPos.x + (m_CubeDimensions.x + m_SpaceBetweenCubesX) * x, m_WallStartPos.y + (m_CubeDimensions.y + m_SpaceBetweenCubesY) * y, 0.0f };

			// Transform the cube
			pCube->Translate(wallPos.x, wallPos.y, wallPos.z);
			pCube->RotateDegrees(0.0f, (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 45.0f, 0.0f);

			// Add the cube to the container of cubes
			m_pCubes.push_back(pCube);
		}
	}

	// Create ground visuals
	auto pGround{ new CubePosColorNorm{ groundDimensions.x, groundDimensions.y, groundDimensions.z, XMFLOAT4{ 0.8f, 0.8f, 0.8f, 1.0f } } };
	AddGameObject(pGround);

	// Create ground physics actor
	auto pGroundActor = pPhysics->createRigidStatic(PxTransform{ PxIdentity });
	const PxBoxGeometry groundGeometry{ groundDimensions.x / 2.0f, groundDimensions.y / 2.0f, groundDimensions.z / 2.0f };

	PxRigidActorExt::createExclusiveShape(*pGroundActor, groundGeometry, *pFloorMaterial);

	// Link ground with the ground actor
	pGround->AttachRigidActor(pGroundActor);

	// Transform the ground
	pGround->Translate(0.0f, -0.5f, 0.0f);
	

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
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Jump), InputTriggerState::pressed, VK_SPACE
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Reset), InputTriggerState::pressed, 'R'
		});

	// Camera setup
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ -15.0f, 7.0f, -40.0f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.3f, -0.1f, 0.5f });
}

void W1_AssignmentScene::Update()
{
	const float torque{ 100.0f };
	const float jumpForce{ 150.0f };

	const XMFLOAT3 rightAxis{ m_SceneContext.GetCamera()->GetRight() };
	const XMFLOAT3 forwardAxis{ m_SceneContext.GetCamera()->GetForward() };

	// Rotate the ball
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right))) m_pRb->addTorque(PxVec3{ forwardAxis.x, forwardAxis.y, forwardAxis.z } * -torque);
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left))) m_pRb->addTorque(PxVec3{ forwardAxis.x, forwardAxis.y, forwardAxis.z } * torque);
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up))) m_pRb->addTorque(PxVec3{ rightAxis.x, rightAxis.y, rightAxis.z } * torque);
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down))) m_pRb->addTorque(PxVec3{ rightAxis.x, rightAxis.y, rightAxis.z } * -torque);

	// Jump
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Jump))) m_pRb->addForce(PxVec3{ 0.0f, jumpForce, 0.0f }, PxForceMode::eIMPULSE);

	// Reset
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Reset)))
	{
		// Reset the ball
		m_pSphere->Translate(0.0f, 1.0f, -30.0f);
		m_pSphere->Rotate(0.0f, 0.0f, 0.0f);

		// Reset the wall of cubes
		for (int x{}; x < m_NrCubesInX; ++x)
		{
			for (int y{}; y < m_NrCubesInY; ++y)
			{
				CubePosColorNorm* pCube{ m_pCubes[x + y * m_NrCubesInX] };

				// Calculate the current position in the wall
				const XMFLOAT3 wallPos{ m_WallStartPos.x + (m_CubeDimensions.x + m_SpaceBetweenCubesX) * x, m_WallStartPos.y + (m_CubeDimensions.y + m_SpaceBetweenCubesY) * y, 0.0f };

				// Transform the cube
				pCube->Translate(wallPos.x, wallPos.y, wallPos.z);
				pCube->RotateDegrees(0.0f, (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 45.0f, 0.0f);
			}
		}
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}
