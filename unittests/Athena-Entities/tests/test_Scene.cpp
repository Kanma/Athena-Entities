#include <UnitTest++.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Serialization.h>
#include <Athena-Core/Data/FileDataStream.h>
#include "../environments/EntitiesTestEnvironment.h"


using namespace Athena::Entities;
using namespace Athena::Data;


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


SUITE(SceneJSONSerialization)
{
    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToObject)
    {
        Scene* pScene = new Scene("test");

        rapidjson::Document document;

        toJSON(pScene, document, document.GetAllocator());

        CHECK(document.IsObject());
        CHECK(document.HasMember("name"));
        CHECK(document.HasMember("components"));
        CHECK(document.HasMember("entities"));

        CHECK_EQUAL("test", document["name"].GetString());


        rapidjson::Value& components = document["components"];

        CHECK(components.IsArray());
        CHECK_EQUAL(0, components.Size());


        rapidjson::Value& entities = document["entities"];

        CHECK(entities.IsArray());
        CHECK_EQUAL(0, entities.Size());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToString)
    {
        Scene* pScene = new Scene("test");
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child");

        pParent->addChild(pChild);

        std::string json = toJSON(pScene);

        std::string reference = readFile("simple.scene");

        CHECK_EQUAL(reference, json);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToFile)
    {
        Scene* pScene = new Scene("test");
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child");

        pParent->addChild(pChild);

        FileDataStream stream(ATHENA_ENTITIES_UNITTESTS_GENERATED_PATH "simple.scene", DataStream::WRITE);
        toJSON(pScene, &stream);
        stream.close();

        std::string written = readFile("simple.scene", "generated");
        std::string reference = readFile("simple.scene");

        CHECK_EQUAL(reference, written);
    }
}


SUITE(SceneJSONDeserialization)
{
    TEST_FIXTURE(EntitiesTestEnvironment, DeserializationFromObject)
    {
        rapidjson::Document document;
        rapidjson::Value array;
        rapidjson::Value entity;
        rapidjson::Value component;
        rapidjson::Value properties;
        rapidjson::Value entry;
        rapidjson::Value field;


        document.SetObject();

        field.SetString("test");
        document.AddMember("name", field, document.GetAllocator());

        field.SetBool(true);
        document.AddMember("enabled", field, document.GetAllocator());


        array.SetArray();
        document.AddMember("components", array, document.GetAllocator());


        entity.SetObject();

        field.SetString("entity");
        entity.AddMember("name", field, document.GetAllocator());

        field.SetBool(true);
        entity.AddMember("enabled", field, document.GetAllocator());


        array.SetArray();

        component.SetObject();

        field.SetString("Transforms://Transforms");
        component.AddMember("id", field, document.GetAllocator());

        properties.SetArray();

        entry.SetObject();

        field.SetString("Athena/Transforms");
        entry.AddMember("__category__", field, document.GetAllocator());

        properties.PushBack(entry, document.GetAllocator());

        entry.SetObject();

        field.SetString("Athena/Component");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetNull();
        entry.AddMember("transforms", field, document.GetAllocator());

        properties.PushBack(entry, document.GetAllocator());

        component.AddMember("properties", properties, document.GetAllocator());

        array.PushBack(component, document.GetAllocator());

        entity.AddMember("components", array, document.GetAllocator());

        array.SetArray();
        array.PushBack(entity, document.GetAllocator());
        document.AddMember("entities", array, document.GetAllocator());


        Scene* pScene = fromJSON(document);
        CHECK(pScene);
        CHECK_EQUAL("test", pScene->getName());
        CHECK_EQUAL(0, pScene->getNbComponents());
        CHECK_EQUAL(1, pScene->getNbEntities());

        unsigned int index = 0;
        Entity* pEntity = pScene->getEntity(index);

        CHECK(pEntity);
        CHECK_EQUAL("entity", pEntity->getName());
        CHECK_EQUAL(1, pEntity->getNbComponents());
        CHECK_EQUAL(0, pEntity->getNbChildren());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, DeserializationFromString)
    {
        std::string reference = readFile("simple.scene");

        Scene* pScene = fromJSON(reference);

        CHECK(pScene);
        CHECK_EQUAL("test", pScene->getName());
        CHECK_EQUAL(0, pScene->getNbComponents());
        CHECK_EQUAL(2, pScene->getNbEntities());

        Entity* pParent = pScene->getEntity("parent");
        CHECK(pParent);
        CHECK_EQUAL(1, pParent->getNbComponents());
        CHECK_EQUAL(1, pParent->getNbChildren());

        unsigned int index = 0;
        Entity* pChild = pParent->getChild(index);
        CHECK(pChild);
        CHECK_EQUAL("child", pChild->getName());
        CHECK_EQUAL(1, pChild->getNbComponents());
        CHECK_EQUAL(0, pChild->getNbChildren());
    }
}
