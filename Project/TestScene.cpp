#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

enum class InputIds
{
	Up,
	Down,
	Left,
	Right
};

void TestScene::Initialize()
{
	// Create a cube object
	m_pBox = new CubePosColorNorm{ 5.0f, 2.0f, 1.0f };
	AddGameObject(m_pBox);

	// Translate the cube
	m_pBox->Translate(0.0f, 1.0f, 0.0f);

	// Camera setup
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.0f, 10.0f, -25.0f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.0f, -0.3f, 1.0f });

	// Create a torus object
	const auto pTorus{ new TorusPosColorNorm{ 8.0f, 50, 1.0f, 10, XMFLOAT4{ Colors::Crimson } } };
	AddGameObject(pTorus);

	// Create a cylinder object
	const auto pCylinder{ new CylinderPosColorNorm{ 1.5f, 10, 4.0f } };
	pTorus->AddChild(pCylinder);
	pCylinder->Translate(0.0f, 8.0f, 0.0f);

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

void TestScene::Update()
{
	// Rotate the cube over time
	const float totalTime = m_SceneContext.GetGameTime()->GetTotal();
	m_pBox->RotateDegrees(0.0f, 90.0f * totalTime, 0.0);

	XMFLOAT3 translation{};
	const float movementSpeed{ 10.0f * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right))) translation.x += movementSpeed;
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left))) translation.x -= movementSpeed;
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up))) translation.y += movementSpeed;
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down))) translation.y -= movementSpeed;

	XMStoreFloat3(&translation, XMLoadFloat3(&translation) + XMLoadFloat3(&m_pBox->GetPosition()));
	m_pBox->Translate(translation.x, translation.y, translation.z);
}

void TestScene::Draw() const
{

}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
