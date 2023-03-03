#include "stdafx.h"
#include "TriggerTestScene.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

	//Ground Plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform({ PxPiDivTwo, {0, 0, 1} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4(Colors::HotPink));
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	//Trigger Left
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -8.f, 0.5f, 0.5f });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ .5f, .5f, .5f }, *pDefaultMaterial);


	//either simulation shape or trigger, not both
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	m_pPhysxScene->addActor(*m_pTriggerLeft);

	//Trigger right
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 8.f, 0.5f, 0.5f });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ .5f, .5f, .5f }, *pDefaultMaterial);


	//either simulation shape or trigger, not both
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	m_pPhysxScene->addActor(*m_pTriggerRight);

}

void TriggerTestScene::Update()
{
	constexpr float force{ 5.f };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}

	if (m_TriggeredLeft)
	{
		m_TriggeredLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({});
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 8.f, 10.f, 0.f }, PxForceMode::eIMPULSE);
	}

	if (m_TriggeredRight)
	{
		m_TriggeredRight = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({});
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ -8.f, 10.f, 0.f }, PxForceMode::eIMPULSE);
	}
}

void TriggerTestScene::Draw() const
{
}

void TriggerTestScene::OnSceneActivated()
{
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (auto i{ 0 }; i < count; i++)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair = pairs[i];

		if (pair.triggerActor == m_pTriggerLeft)//Left trigger
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)//entered
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box LEFT");
				m_TriggeredLeft = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)//exit
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST trigger box LEFT");
			}
		}

		if (pair.triggerActor == m_pTriggerRight)//Right trigger
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)//entered
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box RIGHT");
				m_TriggeredRight = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)//exit
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST trigger box RIGHT");
			}
		}
	}
}