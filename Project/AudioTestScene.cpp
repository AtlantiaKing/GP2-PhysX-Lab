#include "stdafx.h"
#include "AudioTestScene.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"
#include "SoundManager.h"
#include "Logger.h"

void AudioTestScene::Initialize()
{
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

	//INPUT
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction({ Play2D, InputTriggerState::pressed , VK_SPACE });
	pInput->AddInputAction({ Play3D, InputTriggerState::pressed , 'B' });
	pInput->AddInputAction({ IncreaseVolume, InputTriggerState::pressed , VK_UP });
	pInput->AddInputAction({ DecreaseVolume, InputTriggerState::pressed , VK_DOWN });

	const auto pFmod = SoundManager::GetInstance()->GetSystem();

	//Sound 2D
	FMOD::Sound* pSound2D{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Sound 3D
	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Set the bounds where the sound can be heard
	m_pChannel3D->set3DMinMaxDistance(0.f, 100.f);
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)//DirectX vector to FMOD vector
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFmod(PxVec3 v)//PhysX vector to FMOD vector
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Update()
{
	//Logic to move the sphere around
	constexpr float force{ 5.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0, -force });
	}

	//Sound Input
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		//local variable needed to see if channel is paused
		bool bPaused{ false };
		m_pChannel2D->getPaused(&bPaused);
		//Set channel paused to the oposite of paused
		m_pChannel2D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		//local variable needed to see if channel is paused
		bool bPaused{ false };
		m_pChannel3D->getPaused(&bPaused);
		//Set channel paused to the oposite of paused
		m_pChannel3D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume += .1f;
		Clamp(volume, 10.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume -= .1f;
		Clamp(volume, 10.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	//3D Sound Attributes
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFmod(pCam->GetPosition());
	auto forward = ToFmod(pCam->GetForward());
	auto up = ToFmod(pCam->GetUp());

	//calculate the velocity of the camera
	FMOD_VECTOR vel{};
	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_prevCamPos.x) / deltaT;
	vel.y = (pos.y - m_prevCamPos.y) / deltaT;
	vel.z = (pos.z - m_prevCamPos.z) / deltaT;
	m_prevCamPos = pos;

	//Set the attributes for the listener
	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);


	//Get the attributes for the source
	auto spherePos = ToFmod(m_pSphere->GetPosition());
	auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());

	//Set the attributes for the source
	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{
}

void AudioTestScene::OnSceneActivated()
{
}

void AudioTestScene::OnSceneDeactivated()
{
}