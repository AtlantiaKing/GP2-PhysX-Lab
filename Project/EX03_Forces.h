#pragma once

#include "GameScene.h"

class EX03_Forces final : public GameScene
{
public:
	EX03_Forces() :GameScene(L"EX03_Forces") {}
	~EX03_Forces() override = default;
	EX03_Forces(const EX03_Forces& other) = delete;
	EX03_Forces(EX03_Forces&& other) noexcept = delete;
	EX03_Forces& operator=(const EX03_Forces& other) = delete;
	EX03_Forces& operator=(EX03_Forces&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	PxRigidDynamic* m_pRb{};
};
