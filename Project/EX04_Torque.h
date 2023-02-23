#pragma once

#include "GameScene.h"

class EX04_Torque final : public GameScene
{
public:
	EX04_Torque() :GameScene(L"EX04_Torque") {}
	~EX04_Torque() override = default;
	EX04_Torque(const EX04_Torque& other) = delete;
	EX04_Torque(EX04_Torque&& other) noexcept = delete;
	EX04_Torque& operator=(const EX04_Torque& other) = delete;
	EX04_Torque& operator=(EX04_Torque&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	PxRigidDynamic* m_pRb{};
};
