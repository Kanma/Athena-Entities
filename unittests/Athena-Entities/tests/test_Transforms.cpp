#include <UnitTest++.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Serialization.h>
#include "../environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;


SUITE(TransformsComponentCreationTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, DirectCreation)
    {
        ComponentsList* pList = new ComponentsList();

        Transforms* pTransforms = new Transforms("Transforms", pList);

        CHECK_EQUAL(Transforms::TYPE, pTransforms->getType());
        CHECK_EQUAL("Transforms", pTransforms->getName());

        CHECK_EQUAL(COMP_TRANSFORMS, pTransforms->getID().type);
        CHECK_EQUAL("Transforms", pTransforms->getID().strName);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, CreationViaManager)
    {
        ComponentsList list;

        Component* pComponent = pComponentsManager->create(Transforms::TYPE, "Transforms", &list);
        CHECK(pComponent);

        Transforms* pTransforms = Transforms::cast(pComponent);
        CHECK(pTransforms);

        CHECK_EQUAL(Transforms::TYPE, pTransforms->getType());
        CHECK_EQUAL("Transforms", pTransforms->getName());

        CHECK_EQUAL(COMP_TRANSFORMS, pTransforms->getID().type);
        CHECK_EQUAL("Transforms", pTransforms->getID().strName);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, UseParentTransforms)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        CHECK(!pTransforms1->getTransforms());
        CHECK(!pTransforms2->getTransforms());

        pTransforms2->setTransforms(pTransforms1);

        CHECK(!pTransforms1->getTransforms());
        CHECK_EQUAL(pTransforms1, pTransforms2->getTransforms());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RemoveParentTransforms)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);
        pTransforms2->removeTransforms();

        CHECK(!pTransforms1->getTransforms());
        CHECK(!pTransforms2->getTransforms());
    }
}


SUITE(TransformsComponentPositionTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, PositionTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::ZERO.positionEquals(pTransforms->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms->getWorldPosition()));

        pTransforms->setPosition(Vector3(10.0f, 20.0f, 30.0f));

        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getPosition()));
        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, PositionTest2)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms2->setPosition(Vector3(20.0f, 30.0f, 40.0f));

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, PositionTest3)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setPosition(Vector3(20.0f, 30.0f, 40.0f));

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(-30.0f, 40.0f, 30.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, PositionTest4)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->setPosition(Vector3(20.0f, 30.0f, 40.0f));

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(50.0f, 70.0f, 90.0f).positionEquals(pTransforms2->getWorldPosition()));
    }
}


SUITE(TransformsComponentOrientationTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest2)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest3)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->setDirection(Vector3::UNIT_X, Transforms::TS_LOCAL, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest4)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->setDirection(Vector3::UNIT_X, Transforms::TS_PARENT, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest5)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->setDirection(Vector3::UNIT_X, Transforms::TS_WORLD, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest6)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setDirection(Vector3::UNIT_X, Transforms::TS_LOCAL, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest7)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setDirection(Vector3::UNIT_X, Transforms::TS_PARENT, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, OrientationTest8)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setDirection(Vector3::UNIT_X, Transforms::TS_WORLD, Vector3::NEGATIVE_UNIT_Z);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(0.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }
}


SUITE(TransformsComponentScaleTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, ScaleTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms->getWorldScale()));

        pTransforms->setScale(Vector3(2.0f, 3.0f, 4.0f));

        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms->getScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms->getWorldScale()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, ScaleTest2)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms1->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms1->getWorldScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms2->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms2->getWorldScale()));

        pTransforms1->setScale(Vector3(2.0f, 3.0f, 4.0f));
        pTransforms2->setScale(Vector3(2.0f, 3.0f, 4.0f));

        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms1->getScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms1->getWorldScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms2->getScale()));
        CHECK(Vector3(4.0f, 9.0f, 16.0f).positionEquals(pTransforms2->getWorldScale()));
    }
}


SUITE(TransformsComponentTranslationTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::ZERO.positionEquals(pTransforms->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms->getWorldPosition()));

        pTransforms->translate(Vector3(10.0f, 20.0f, 30.0f), Transforms::TS_LOCAL);

        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getPosition()));
        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest2)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::ZERO.positionEquals(pTransforms->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms->getWorldPosition()));

        pTransforms->translate(Vector3(10.0f, 20.0f, 30.0f), Transforms::TS_PARENT);

        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getPosition()));
        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest3)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::ZERO.positionEquals(pTransforms->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms->getWorldPosition()));

        pTransforms->translate(Vector3(10.0f, 20.0f, 30.0f), Transforms::TS_WORLD);

        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getPosition()));
        CHECK(Vector3(10.0f, 20.0f, 30.0f).positionEquals(pTransforms->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest4)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_LOCAL);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest5)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_PARENT);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest6)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_WORLD);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest7)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_LOCAL);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(-40.0f, 30.0f, 20.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(-10.0f, 40.0f, -30.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest8)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_PARENT);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(-30.0f, 40.0f, 30.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest9)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_WORLD);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(40.0f, 30.0f, -20.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest10)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_LOCAL);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(50.0f, 70.0f, 90.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest11)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_PARENT);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(20.0f, 30.0f, 40.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(50.0f, 70.0f, 90.0f).positionEquals(pTransforms2->getWorldPosition()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, TranslationTest12)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3::ZERO.positionEquals(pTransforms2->getWorldPosition()));

        pTransforms1->setPosition(Vector3(10.0f, 10.0f, 10.0f));
        pTransforms1->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->setScale(Vector3(2.0f, 2.0f, 2.0f));
        pTransforms2->translate(Vector3(20.0f, 30.0f, 40.0f), Transforms::TS_WORLD);

        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getPosition()));
        CHECK(Vector3(10.0f, 10.0f, 10.0f).positionEquals(pTransforms1->getWorldPosition()));
        CHECK(Vector3(10.0f, 15.0f, 20.0f).positionEquals(pTransforms2->getPosition()));
        CHECK(Vector3(30.0f, 40.0f, 50.0f).positionEquals(pTransforms2->getWorldPosition()));
    }
}


SUITE(TransformsComponentRotationsTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_LOCAL);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest2)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_PARENT);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest3)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Quaternion::IDENTITY.equals(pTransforms->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms->getWorldOrientation(), Degree(0.1f)));

        pTransforms->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_WORLD);

        CHECK(pTransforms->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest4)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_LOCAL);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest5)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_PARENT);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest6)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_Y, Degree(-90.0f), Transforms::TS_WORLD);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-180.0f), Vector3::UNIT_Y), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest7)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_X, Degree(-90.0f), Transforms::TS_LOCAL);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_X), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y) * Quaternion(Degree(-90.0f), Vector3::UNIT_X), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest8)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_X, Degree(-90.0f), Transforms::TS_PARENT);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_X), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y) * Quaternion(Degree(-90.0f), Vector3::UNIT_X), Degree(0.1f)));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, RotationTest9)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms1->getWorldOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getOrientation(), Degree(0.1f)));
        CHECK(Quaternion::IDENTITY.equals(pTransforms2->getWorldOrientation(), Degree(0.1f)));

        pTransforms1->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
        pTransforms2->rotate(Vector3::UNIT_X, Degree(-90.0f), Transforms::TS_WORLD);

        CHECK(pTransforms1->getOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms1->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y), Degree(0.1f)));
        CHECK(pTransforms2->getOrientation().equals(Quaternion(Degree(90.0f), Vector3::UNIT_Z), Degree(0.1f)));
        CHECK(pTransforms2->getWorldOrientation().equals(Quaternion(Degree(-90.0f), Vector3::UNIT_Y) * Quaternion(Degree(90.0f), Vector3::UNIT_Z), Degree(0.1f)));
    }
}


SUITE(TransformsComponentScalingTests)
{
    TEST_FIXTURE(EntitiesTestEnvironment, ScalingTest1)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms", &list);

        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms->getWorldScale()));

        pTransforms->scale(Vector3(2.0f, 3.0f, 4.0f));

        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms->getScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms->getWorldScale()));
    }


    TEST_FIXTURE(EntitiesTestEnvironment, ScalingTest2)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms1->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms1->getWorldScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms2->getScale()));
        CHECK(Vector3::UNIT_SCALE.positionEquals(pTransforms2->getWorldScale()));

        pTransforms1->setScale(Vector3(2.0f, 3.0f, 4.0f));
        pTransforms2->scale(Vector3(2.0f, 3.0f, 4.0f));

        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms1->getScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms1->getWorldScale()));
        CHECK(Vector3(2.0f, 3.0f, 4.0f).positionEquals(pTransforms2->getScale()));
        CHECK(Vector3(4.0f, 9.0f, 16.0f).positionEquals(pTransforms2->getWorldScale()));
    }
}


SUITE(TransformsJSONSerialization)
{
    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToObject)
    {
        ComponentsList list;

        Transforms* pTransforms = new Transforms("Transforms1", &list);

        rapidjson::Document document;

        toJSON(pTransforms, document, document.GetAllocator());

        CHECK(document.IsObject());
        CHECK(document.HasMember("id"));
        CHECK(document.HasMember("properties"));

        CHECK_EQUAL("Transforms://Transforms1", document["id"].GetString());

        rapidjson::Value& properties = document["properties"];

        CHECK(properties.IsArray());
        CHECK_EQUAL(2, properties.Size());

        rapidjson::Value& category = properties[(rapidjson::SizeType) 0];

        CHECK(category.IsObject());
        CHECK_EQUAL("Athena/Transforms", category["__category__"].GetString());

        CHECK(category.HasMember("position"));
        CHECK(category.HasMember("orientation"));
        CHECK(category.HasMember("scale"));
        CHECK(category.HasMember("inheritOrientation"));
        CHECK(category.HasMember("inheritScale"));

        category = properties[1];

        CHECK(category.IsObject());
        CHECK_EQUAL("Athena/Component", category["__category__"].GetString());

        CHECK(!category.HasMember("transforms"));
    }

    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToObjectWithReferenceToAnotherTransforms)
    {
        ComponentsList list;

        Transforms* pTransforms1 = new Transforms("Transforms1", &list);
        Transforms* pTransforms2 = new Transforms("Transforms2", &list);

        pTransforms2->setTransforms(pTransforms1);

        rapidjson::Document document;

        toJSON(pTransforms2, document, document.GetAllocator());

        CHECK(document.IsObject());
        CHECK(document.HasMember("id"));
        CHECK(document.HasMember("properties"));

        CHECK_EQUAL("Transforms://Transforms2", document["id"].GetString());

        rapidjson::Value& properties = document["properties"];

        CHECK(properties.IsArray());
        CHECK_EQUAL(2, properties.Size());

        rapidjson::Value& category = properties[(rapidjson::SizeType) 0];

        CHECK(category.IsObject());
        CHECK_EQUAL("Athena/Transforms", category["__category__"].GetString());

        CHECK(category.HasMember("position"));
        CHECK(category.HasMember("orientation"));
        CHECK(category.HasMember("scale"));
        CHECK(category.HasMember("inheritOrientation"));
        CHECK(category.HasMember("inheritScale"));

        category = properties[1];

        CHECK(category.IsObject());
        CHECK_EQUAL("Athena/Component", category["__category__"].GetString());

        CHECK(category.HasMember("transforms"));
        CHECK_EQUAL("Transforms://Transforms1", category["transforms"].GetString());
    }
}
