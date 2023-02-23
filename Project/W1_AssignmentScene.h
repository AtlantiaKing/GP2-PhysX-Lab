#pragma once

#include "GameScene.h"

class SpherePosColorNorm;
class CubePosColorNorm;

class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	PxRigidDynamic* m_pRb{};
	SpherePosColorNorm* m_pSphere{};
	std::vector<CubePosColorNorm*> m_pCubes{};

	const XMFLOAT3 m_CubeDimensions{ 1.0f, 1.0f, 1.0f };
	const float m_SpaceBetweenCubesX{ 0.2f };
	const float m_SpaceBetweenCubesY{ 0.05f };
	const XMFLOAT3 m_WallStartPos{ (-m_CubeDimensions.x - m_SpaceBetweenCubesX) * 4.5f, 0.5f, 0.0f };
	const int m_NrCubesInX{ 10 };
	const int m_NrCubesInY{ 10 };
};
