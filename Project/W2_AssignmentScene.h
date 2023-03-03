#pragma once
#include "GameScene.h"

class SpherePosColorNorm;

class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	
private:
	PxRigidStatic* m_pLeftNotchTrigger{};
	PxRigidStatic* m_pRightNotchTrigger{};
	PxRigidDynamic* m_pLeftBox{};
	PxRigidDynamic* m_pRightBox{};
	PxRigidDynamic* m_pLeftHatchBox{};
	PxRigidDynamic* m_pRightHatchBox{};
	SpherePosColorNorm* m_pSphere{};
	bool m_LeftHatchOpen{};
	bool m_RightHatchOpen{};
	FMOD::Sound* m_pBellSound{};
	FMOD::Channel* m_pChannel2D{ nullptr };
};