#include "stdafx.h"
#include "PhysicsMeshTestScene.h"
#include "ContentManager.h"
#include "MeshObject.h"
#include "PhysxManager.h"

void PhysicsMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, 1.2f);

	//Ground Plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform({ PxPiDivTwo, {0, 0, 1} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);


	//Convex chair
	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairConvex);

	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);

	//center of mass is standard the pivot point in the 3D editor
	//set new center of mass
	PxVec3 massLocalPos{ 0.f, 5.f, 0.f };
	//change density and center of mass
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPos);

	m_pChairConvex->AttachRigidActor(pConvexActor);
	m_pChairConvex->Translate(0.f, 12.f, 0.f);


	//Triangle Chair
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTriangleActor);
	AddGameObject(m_pChairTriangle);

	m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	m_KinematicPosition = { 10.f, 0.f, 0.f };


}

void PhysicsMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(10.f, 0.f, 0.f);
		m_pChairConvex->Rotate(0.f, 0.f, 0.f);

		m_KinematicPosition = { 10.f, 0.f, 0.f };
		m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_KinematicPosition.x += 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
	}

}

void PhysicsMeshTestScene::Draw() const
{
}

void PhysicsMeshTestScene::OnSceneActivated()
{
}

void PhysicsMeshTestScene::OnSceneDeactivated()
{
}