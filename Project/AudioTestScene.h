#pragma once
#include "GameScene.h"
class AudioTestScene final : public GameScene
{
public:
	AudioTestScene() :GameScene(L"AudioTestScene") {}
	~AudioTestScene() override = default;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pSphere{ nullptr };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Channel* m_pChannel3D{ nullptr };

	//Needed to calculate the velocity of the camera
	FMOD_VECTOR m_prevCamPos{};

	enum InputIds
	{
		Play2D,
		Play3D,
		IncreaseVolume,
		DecreaseVolume
	};
};