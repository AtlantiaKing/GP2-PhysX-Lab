#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "ContentManager.h"
#include "MeshObject.h"
#include "PhysxManager.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "SoundManager.h"

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, 0.0f);
	const XMFLOAT4 redColor{ 0.5f, 0.0f, 0.0f, 1.0f };
	const XMFLOAT4 blueColor{ 0.0f, 0.0f, 0.5f, 1.0f };
	const XMFLOAT4 grayColor{ 0.5f, 0.5f, 0.5f, 1.0f };
	const XMFLOAT3 notchBoxSize{ 1.5f, 0.4f, 6.0f };
	const XMFLOAT3 cubeSize{ 2.0f, 2.0f, 2.0f };
	const XMFLOAT3 hatchSize{ 2.0f, 0.2f, 6.0f };

	//Ground Plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform({ PxPiDivTwo, {0, 0, 1} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Level
	auto pLevelObject = new MeshObject(L"Resources/Meshes/Level.ovm");

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pTriangleActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	pLevelObject->AttachRigidActor(pTriangleActor);
	AddGameObject(pLevelObject);

	// Left notch
	auto pLeftNotch{ new CubePosColorNorm{ notchBoxSize.x, notchBoxSize.y, notchBoxSize.z, blueColor } };
	AddGameObject(pLeftNotch);

	m_pLeftNotchTrigger = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pLeftNotchShape{ PxRigidActorExt::createExclusiveShape(*m_pLeftNotchTrigger, PxBoxGeometry{ notchBoxSize.x / 2.0f, notchBoxSize.y / 2.0f, notchBoxSize.z / 2.0f }, *pDefaultMaterial) };
	pLeftNotch->AttachRigidActor(m_pLeftNotchTrigger);

	pLeftNotchShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pLeftNotchShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	pLeftNotch->Translate(-7.5f, 2.0f + notchBoxSize.y / 2.0f, 0.0f);
	
	// Right notch
	auto pRightNotch{ new CubePosColorNorm{ notchBoxSize.x, notchBoxSize.y, notchBoxSize.z, redColor } };
	AddGameObject(pRightNotch);

	m_pRightNotchTrigger = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pRightNotchShape{ PxRigidActorExt::createExclusiveShape(*m_pRightNotchTrigger, PxBoxGeometry{ notchBoxSize.x / 2.0f, notchBoxSize.y / 2.0f, notchBoxSize.z / 2.0f }, *pDefaultMaterial) };
	
	pRightNotchShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pRightNotchShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	pRightNotch->AttachRigidActor(m_pRightNotchTrigger);

	pRightNotch->Translate(6.5f, 2.0f + notchBoxSize.y / 2.0f, 0.0f);

	// Left cube
	auto pLeftCube{ new CubePosColorNorm{ cubeSize.x, cubeSize.y, cubeSize.z, blueColor } };
	AddGameObject(pLeftCube);

	m_pLeftBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*m_pLeftBox, PxBoxGeometry{ cubeSize.x / 2.0f, cubeSize.y / 2.0f, cubeSize.z / 2.0f }, *pDefaultMaterial);
	PxRigidBodyExt::updateMassAndInertia(*m_pLeftBox, 0.01f);

	pLeftCube->AttachRigidActor(m_pLeftBox);

	pLeftCube->Translate(-3.5f, 5.0f, 0.0f);

	// Right cube
	auto pRightCube{ new CubePosColorNorm{ cubeSize.x, cubeSize.y, cubeSize.z, redColor } };
	AddGameObject(pRightCube);

	m_pRightBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*m_pRightBox, PxBoxGeometry{ cubeSize.x / 2.0f, cubeSize.y / 2.0f, cubeSize.z / 2.0f }, *pDefaultMaterial);
	PxRigidBodyExt::updateMassAndInertia(*m_pRightBox, 0.01f);

	pRightCube->AttachRigidActor(m_pRightBox);

	pRightCube->Translate(3.0f, 5.0f, 0.0f);

	//Sphere
	m_pSphere = new SpherePosColorNorm{ 1.f, 10, 10, grayColor };
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 5.f, 0.f);

	auto d6joint{ PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity }) };
	d6joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	// Left hatch
	auto pLeftHatch{ new CubePosColorNorm{ hatchSize.x, hatchSize.y, hatchSize.z, blueColor } };
	AddGameObject(pLeftHatch);

	m_pLeftHatchBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	m_pLeftHatchBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*m_pLeftHatchBox, PxBoxGeometry{ hatchSize.x / 2.0f, hatchSize.y / 2.0f, hatchSize.z / 2.0f }, *pDefaultMaterial);

	pLeftHatch->AttachRigidActor(m_pLeftHatchBox);

	pLeftHatch->Translate(-9.0f, 17.0f, 0.0f);

	PxTransform localFrame{ PxVec3{ 1.0f, 0.0f, 0.0f }, PxQuat{ PxHalfPi, PxVec3{ 0.0f, 1.0f, 0.0f } } };
	PxTransform globalFrame{ PxVec3{ -9.0f, 17.0f, 0.0f } };
	PxTransform jointFrame = globalFrame * localFrame;
	PxRevoluteJointCreate(*pPhysX,
		m_pLeftHatchBox, localFrame,
		NULL, jointFrame);
	
	// Right hatch
	auto pRightHatch{ new CubePosColorNorm{ hatchSize.x, hatchSize.y, hatchSize.z, blueColor } };
	AddGameObject(pRightHatch);

	m_pRightHatchBox = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	m_pRightHatchBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*m_pRightHatchBox, PxBoxGeometry{ hatchSize.x / 2.0f, hatchSize.y / 2.0f, hatchSize.z / 2.0f }, * pDefaultMaterial);

	pRightHatch->AttachRigidActor(m_pRightHatchBox);

	pRightHatch->Translate(9.0f, 17.0f, 0.0f);

	localFrame = PxTransform{ PxVec3(-1.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)) };
	globalFrame = PxTransform{ PxVec3(9.0f, 17.0f, 0.0f) };
	jointFrame = globalFrame * localFrame;
	PxRevoluteJointCreate(*pPhysX,
		m_pRightHatchBox, localFrame,
		NULL, jointFrame);

	// Spheres on top
	auto pLeftTopSphere{ new SpherePosColorNorm{ 1.f, 10, 10, grayColor } };
	AddGameObject(pLeftTopSphere);

	const auto pSphereActorTopLeft = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActorTopLeft, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	pLeftTopSphere->AttachRigidActor(pSphereActorTopLeft);

	pLeftTopSphere->Translate(-10.f, 20.f, 0.f);

	auto pRightTopSphere{ new SpherePosColorNorm{ 1.f, 10, 10, grayColor } };
	AddGameObject(pRightTopSphere);

	const auto pSphereActorTopRight = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActorTopRight, PxSphereGeometry{ 1.f }, * pDefaultMaterial);
	pRightTopSphere->AttachRigidActor(pSphereActorTopRight);

	pRightTopSphere->Translate(10.f, 20.f, 0.f);

	// Sound
	const auto pFmod = SoundManager::GetInstance()->GetSystem();

	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_DEFAULT, nullptr, &m_pBellSound);
	SoundManager::GetInstance()->ErrorCheck(result);
}

void W2_AssignmentScene::Update()
{
	constexpr float force{ 5.0f };

	// Move sphere
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.0f, 400, 0.0f });
	}

	// Open hatches when the triggers are hit
	if(m_LeftHatchOpen)
	{
		m_pLeftHatchBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		m_pLeftHatchBox->addForce({ 0.0f, -10.0f, 0.0f });
		m_LeftHatchOpen = false;
	}
	if (m_RightHatchOpen)
	{
		m_pRightHatchBox->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		m_pRightHatchBox->addForce({ 0.0f, -10.0f, 0.0f });
		m_RightHatchOpen = false;
	}
}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::OnSceneActivated()
{
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (auto i{ 0 }; i < count; i++)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair = pairs[i];

		if(pair.status != PxPairFlag::eNOTIFY_TOUCH_FOUND) continue;

		// Left trigger
		if (pair.triggerActor == m_pLeftNotchTrigger && pair.otherActor == m_pLeftBox)
		{
			// Open the hatch
			m_LeftHatchOpen = true;

			// Play sound
			FMOD_RESULT result = SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
			SoundManager::GetInstance()->ErrorCheck(result);
		}

		//Right trigger
		if (pair.triggerActor == m_pRightNotchTrigger && pair.otherActor == m_pRightBox)
		{
			// Open the hatch
			m_RightHatchOpen = true;

			// Play sound
			FMOD_RESULT result = SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
			SoundManager::GetInstance()->ErrorCheck(result);
		}
	}
}
