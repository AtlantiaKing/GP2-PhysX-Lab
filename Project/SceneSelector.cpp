#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

#define W01
//#define W02

#ifdef W01
//#include "TestScene.h"
//#include "PhysXTestScene.h"
#include "EX01_BouncingBalls.h"
#include "EX02_FrictionTest.h"
#include "EX03_Forces.h"

#endif

#ifdef W02

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
#endif

#ifdef W02

#endif

	}
}

