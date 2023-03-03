#pragma once
#include "GameScene.h"
class PhysicsMeshTestScene final : public GameScene
{
public:
	PhysicsMeshTestScene() :GameScene(L"PhysicsMeshTestScene") {}
	~PhysicsMeshTestScene() override = default;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pChairConvex{ nullptr };

	GameObject* m_pChairTriangle{ nullptr };

	PxVec3 m_KinematicPosition{};
};