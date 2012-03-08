#include <UnitTest++.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Entity.h>
#include "../environments/EntitiesTestEnvironment.h"


using namespace Athena;
using namespace Athena::Entities;
using namespace Athena::Utils;


SUITE(ComponentsListTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, Creation)
    {
        ComponentsList* pList = new ComponentsList();

        CHECK_EQUAL(0, pList->getNbComponents());

        delete pList;
    }


    TEST_FIXTURE(EntitiesTestEnvironment, EntityAssignment)
    {
        ComponentsList* pList = new ComponentsList();

        CHECK(!pList->getEntity());

        Entity* pEntity = pScene->create("test");
        pList->_setEntity(pEntity);

        CHECK_EQUAL(pEntity, pList->getEntity());

        delete pList;
        pScene->destroy(pEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, AddComponent)
    {
        ComponentsList* pList = new ComponentsList();

        CHECK_EQUAL(0, pList->getNbComponents());

        Component* pComponent = new Component("Component", pList);

        CHECK_EQUAL(1, pList->getNbComponents());

        unsigned int uiIndex = 0;
        CHECK(pComponent == pList->getComponent(uiIndex));
        CHECK(pComponent == pList->getComponent(pComponent->getID()));

        CHECK_EQUAL("", pComponent->getID().strEntity);

        delete pList;
    }


    TEST_FIXTURE(EntitiesTestEnvironment, MemberComponentIDContainsEntityName)
    {
        ComponentsList* pList = new ComponentsList();

        Entity* pEntity = pScene->create("test");
        pList->_setEntity(pEntity);

        Component* pComponent = new Component("Component", pList);

        CHECK_EQUAL(pEntity->getName(), pComponent->getID().strEntity);

        delete pList;

        pScene->destroy(pEntity);
    }
}
