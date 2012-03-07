#include <UnitTest++.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/ComponentsList.h>
#include "environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;


TEST(ComponentsManager_Singleton)
{
    CHECK(!ComponentsManager::getSingletonPtr());

    ComponentsManager* pComponentsManager = new ComponentsManager();
    CHECK(pComponentsManager == ComponentsManager::getSingletonPtr());

    delete pComponentsManager;
    CHECK(!ComponentsManager::getSingletonPtr());
}


SUITE(ComponentsManagerTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, ComponentCreation)
    {
        ComponentsList* pList = new ComponentsList();

        Component* pComponent = pComponentsManager->create(Component::TYPE, "test", pList);

        CHECK(pComponent);
        CHECK(pComponent->getType() == Component::TYPE);

        delete pList;
    }


    TEST_FIXTURE(EntitiesTestEnvironment, ComponentDestruction)
    {
        ComponentsList* pList = new ComponentsList();

        Component* pComponent = pComponentsManager->create(Component::TYPE, "test", pList);

        CHECK_EQUAL(1, pList->getNbComponents());

        pComponentsManager->destroy(pComponent);

        CHECK_EQUAL(0, pList->getNbComponents());

        delete pList;
    }
}
