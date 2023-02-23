#pragma once

#include "GameScene.h"

class EX01_BouncingBalls final : public GameScene
{
public:
	EX01_BouncingBalls() :GameScene(L"EX01_BouncingBalls") {}
	~EX01_BouncingBalls() override = default;
	EX01_BouncingBalls(const EX01_BouncingBalls& other) = delete;
	EX01_BouncingBalls(EX01_BouncingBalls&& other) noexcept = delete;
	EX01_BouncingBalls& operator=(const EX01_BouncingBalls& other) = delete;
	EX01_BouncingBalls& operator=(EX01_BouncingBalls&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
};

