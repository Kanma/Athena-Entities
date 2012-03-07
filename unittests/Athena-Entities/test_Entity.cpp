#include <UnitTest++.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Transforms.h>
#include "environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;


SUITE(EntityTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, Creation)
    {
        Entity* pEntity = pScene->create("test");
        CHECK_EQUAL("test", pEntity->getName());
        CHECK(pEntity->getComponentsList());
        CHECK(pEntity->getSignalsList());
        CHECK(pEntity->getTransforms());
        pScene->destroy(pEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, AddTransforms)
    {
        Entity* pEntity = pScene->create("test");

        CHECK(pEntity->getTransforms());

        Transforms* pTransforms = new Transforms("T2", pEntity->getComponentsList());

        CHECK_EQUAL(pTransforms->getTransforms(), pEntity->getTransforms());

        pScene->destroy(pEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, CreationWithParent)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child", pParent);

        CHECK(!pParent->getParent());
        CHECK(pChild->getParent() == pParent);
        CHECK_EQUAL(1, pParent->getNbChildren());

        Entity::ChildrenIterator iter = pParent->getChildrenIterator();
        CHECK(iter.hasMoreElements());
        CHECK(iter.getNext() == pChild);
        CHECK(!iter.hasMoreElements());

        iter = pChild->getChildrenIterator();
        CHECK(!iter.hasMoreElements());

        CHECK(pParent->getTransforms());
        CHECK(pChild->getTransforms());
        CHECK_EQUAL(pParent->getTransforms(), pChild->getTransforms()->getTransforms());

        pScene->destroy(pChild);
        pScene->destroy(pParent);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, AddChild)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child");

        CHECK(!pParent->getParent());
        CHECK(!pChild->getParent());
        CHECK_EQUAL(0, pParent->getNbChildren());

        pParent->addChild(pChild);

        CHECK(pChild->getParent() == pParent);
        CHECK_EQUAL(1, pParent->getNbChildren());

        Entity::ChildrenIterator iter = pParent->getChildrenIterator();
        CHECK(iter.hasMoreElements());
        CHECK(iter.getNext() == pChild);
        CHECK(!iter.hasMoreElements());

        CHECK(pParent->getTransforms());
        CHECK(pChild->getTransforms());
        CHECK_EQUAL(pParent->getTransforms(), pChild->getTransforms()->getTransforms());

        pScene->destroy(pChild);
        pScene->destroy(pParent);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RemoveChild)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child", pParent);

        CHECK(pChild->getParent() == pParent);
        CHECK_EQUAL(1, pParent->getNbChildren());

        pParent->removeChild(pChild);

        CHECK(!pChild->getParent());
        CHECK_EQUAL(0, pParent->getNbChildren());

        CHECK(pParent->getTransforms());
        CHECK(pChild->getTransforms());
        CHECK(!pChild->getTransforms()->getTransforms());

        pScene->destroy(pChild);
        pScene->destroy(pParent);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, ParentDestruction)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child", pParent);

        pScene->destroy(pParent);

        Entity* pRetrievedEntity = pScene->getEntity("child");
        CHECK(!pRetrievedEntity);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, DestroyAllChildren)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child", pParent);

        CHECK_EQUAL(1, pParent->getNbChildren());

        pParent->destroyAllChildren();

        CHECK_EQUAL(0, pParent->getNbChildren());

        Entity* pRetrievedEntity = pScene->getEntity("child");
        CHECK(!pRetrievedEntity);

        pScene->destroy(pParent);
    }
}
