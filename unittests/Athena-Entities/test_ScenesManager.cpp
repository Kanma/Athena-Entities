#include <UnitTest++.h>
#include <Athena-Entities/ScenesManager.h>
#include "environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;


TEST(ScenesManager_Singleton)
{
	CHECK(!ScenesManager::getSingletonPtr());
	
	ScenesManager* pScenesManager = new ScenesManager();
	CHECK(pScenesManager);
	CHECK(pScenesManager == ScenesManager::getSingletonPtr());

	delete pScenesManager;
	CHECK(!ScenesManager::getSingletonPtr());
}


SUITE(ScenesManagerTests)
{
	TEST_FIXTURE(EntitiesTestEnvironment, TestFixture)
	{
		CHECK(pScenesManager);
		CHECK(pComponentsManager);
	}


	TEST_FIXTURE(EntitiesTestEnvironment, SceneRetrieval)
	{
		Scene* pScene			= pScenesManager->create("test");
		Scene* pRetrievedScene	= pScenesManager->getScene("test");

		CHECK(pRetrievedScene);
		CHECK(pRetrievedScene == pScene);

		delete pScene;
	}


	TEST_FIXTURE(EntitiesTestEnvironment, SceneRetrievalFailure)
	{
		Scene* pRetrievedScene = pScenesManager->getScene("test");

		CHECK(!pRetrievedScene);
	}


	TEST_FIXTURE(EntitiesTestEnvironment, SceneDestruction)
	{
		Scene* pScene = pScenesManager->create("test");
		delete pScene;

		Scene* pRetrievedScene = pScenesManager->getScene("test");
		CHECK(!pRetrievedScene);
	}


	TEST_FIXTURE(EntitiesTestEnvironment, SceneDestructionFromManager)
	{
		Scene* pScene = pScenesManager->create("test");
		pScenesManager->destroy(pScene);

		Scene* pRetrievedScene = pScenesManager->getScene("test");
		CHECK(!pRetrievedScene);
	}


	TEST_FIXTURE(EntitiesTestEnvironment, SceneDestructionFromManagerWithName)
	{
		pScenesManager->create("test");
		pScenesManager->destroy("test");

		Scene* pRetrievedScene = pScenesManager->getScene("test");
		CHECK(!pRetrievedScene);
	}
}
