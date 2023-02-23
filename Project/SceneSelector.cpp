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
#endif

#ifdef W02

#endif

	}
}

