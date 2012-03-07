#include <UnitTest++.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Entity.h>
#include "environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;


SUITE(SceneTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, TestFixture)
    {
        CHECK(pScenesManager);
        CHECK(pComponentsManager);
        CHECK(pScene);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityRetrieval)
    {
        Entity* pEntity             = pScene->create("test");
        Entity* pRetrievedEntity    = pScene->getEntity("test");

        CHECK(pRetrievedEntity);
        CHECK(pRetrievedEntity == pEntity);

        pScene->destroy(pEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityRetrievalFailure)
    {
        Entity* pRetrievedEntity = pScene->getEntity("test");

        CHECK(!pRetrievedEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityDestruction)
    {
        Entity* pEntity = pScene->create("test");
        pScene->destroy(pEntity);

        Entity* pRetrievedEntity = pScene->getEntity("test");
        CHECK(!pRetrievedEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityDestructionFromManagerWithName)
    {
        pScene->create("test");
        pScene->destroy("test");

        Entity* pRetrievedEntity = pScene->getEntity("test");
        CHECK(!pRetrievedEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityTransfer)
    {
        Scene*  pScene2 = new Scene("second");
        Entity* pEntity = pScene->create("test");

        CHECK(pEntity->getScene() == pScene);
        CHECK_EQUAL(1, pScene->getNbEntities());
        CHECK_EQUAL(0, pScene2->getNbEntities());

        pScene2->transfer(pEntity);

        CHECK(pEntity->getScene() == pScene2);
        CHECK_EQUAL(0, pScene->getNbEntities());
        CHECK_EQUAL(1, pScene2->getNbEntities());
        CHECK_EQUAL(pEntity, pScene2->getEntity("test"));

        pScene2->destroy(pEntity);
        delete pScene2;
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityTransferWithName)
    {
        Scene*  pScene2 = new Scene("second");
        Entity* pEntity = pScene->create("test");

        CHECK(pEntity->getScene() == pScene);
        CHECK_EQUAL(1, pScene->getNbEntities());
        CHECK_EQUAL(0, pScene2->getNbEntities());

        pScene2->transfer("test", pScene);

        CHECK(pEntity->getScene() == pScene2);
        CHECK_EQUAL(0, pScene->getNbEntities());
        CHECK_EQUAL(1, pScene2->getNbEntities());
        CHECK_EQUAL(pEntity, pScene2->getEntity("test"));

        pScene2->destroy(pEntity);
        delete pScene2;
    }

    TEST_FIXTURE(EntitiesTestEnvironment, NoMainComponentByDefault)
    {
        CHECK(!pScene->getMainComponent(COMP_VISUAL));
        CHECK(!pScene->getMainComponent(COMP_AUDIO));
        CHECK(!pScene->getMainComponent(COMP_PHYSICAL));
    }
}
