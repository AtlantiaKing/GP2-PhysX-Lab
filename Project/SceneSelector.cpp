#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

//#define W01
//#define W02
#define M01

#ifdef W01
//#include "TestScene.h"
//#include "PhysXTestScene.h"
#include "EX01_BouncingBalls.h"
#include "EX02_FrictionTest.h"
#include "EX03_Forces.h"
#include "EX04_Torque.h"
#include "W1_AssignmentScene.h"

#endif

#ifdef W02
//#include "PhysicsMeshTestScene.h"
//#include "TriggerTestScene.h"
//#include "AudioTestScene.h"
#include "W2_AssignmentScene.h"
#endif

#ifdef M01
#include "W1_AssignmentScene.h"
#include "W2_AssignmentScene.h"
#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		/*pSceneManager->AddGameScene(new TestScene());
		pSceneManager->AddGameScene(new PhysXTestScene());*/
		pSceneManager->AddGameScene(new EX01_BouncingBalls());
		pSceneManager->AddGameScene(new EX02_FrictionTest());
		pSceneManager->AddGameScene(new EX03_Forces());
		pSceneManager->AddGameScene(new EX04_Torque());
		pSceneManager->AddGameScene(new W1_AssignmentScene());
#endif

#ifdef W02
		/*pSceneManager->AddGameScene(new PhysicsMeshTestScene());
		pSceneManager->AddGameScene(new TriggerTestScene());
		pSceneManager->AddGameScene(new AudioTestScene());*/
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif

#ifdef M01
		pSceneManager->AddGameScene(new W1_AssignmentScene());
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif

	}
}

