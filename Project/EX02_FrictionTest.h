#pragma once

#include "GameScene.h"

class EX02_FrictionTest final : public GameScene
{
public:
	EX02_FrictionTest() :GameScene(L"EX02_FrictionTest") {}
	~EX02_FrictionTest() override = default;
	EX02_FrictionTest(const EX02_FrictionTest& other) = delete;
	EX02_FrictionTest(EX02_FrictionTest&& other) noexcept = delete;
	EX02_FrictionTest& operator=(const EX02_FrictionTest& other) = delete;
	EX02_FrictionTest& operator=(EX02_FrictionTest&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
};

