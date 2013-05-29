#include <UnitTest++.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Serialization.h>
#include "../environments/EntitiesTestEnvironment.h"


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

        Entity::tEntitiesIterator iter = pParent->getChildrenIterator();
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

        Entity::tEntitiesIterator iter = pParent->getChildrenIterator();
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


SUITE(EntityJSONSerialization)
{
    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToObject)
    {
        Entity* pEntity = pScene->create("test");

        rapidjson::Document document;

        toJSON(pEntity, document, document.GetAllocator());

        CHECK(document.IsObject());
        CHECK(document.HasMember("name"));
        CHECK(document.HasMember("components"));
        CHECK(document.HasMember("children"));

        CHECK_EQUAL("test", document["name"].GetString());


        rapidjson::Value& components = document["components"];

        CHECK(components.IsArray());
        CHECK_EQUAL(1, components.Size());

        rapidjson::Value& component = components[(rapidjson::SizeType) 0];

        CHECK(component.IsObject());
        CHECK_EQUAL("Transforms://Transforms", component["id"].GetString());


        rapidjson::Value& children = document["children"];

        CHECK(children.IsArray());
        CHECK_EQUAL(0, children.Size());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToObjectWithChild)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child");

        pParent->addChild(pChild);

        rapidjson::Document document;

        toJSON(pParent, document, document.GetAllocator());

        CHECK(document.IsObject());
        CHECK(document.HasMember("name"));
        CHECK(document.HasMember("components"));
        CHECK(document.HasMember("children"));

        CHECK_EQUAL("parent", document["name"].GetString());


        rapidjson::Value& components = document["components"];

        CHECK(components.IsArray());
        CHECK_EQUAL(1, components.Size());

        rapidjson::Value& component = components[(rapidjson::SizeType) 0];

        CHECK(component.IsObject());
        CHECK_EQUAL("Transforms://Transforms", component["id"].GetString());


        rapidjson::Value& children = document["children"];

        CHECK(children.IsArray());
        CHECK_EQUAL(1, children.Size());

        rapidjson::Value& child = children[(rapidjson::SizeType) 0];

        CHECK(child.IsObject());
        CHECK_EQUAL("child", child["name"].GetString());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToString)
    {
        Entity* pEntity = pScene->create("test");

        std::string json = toJSON(pEntity);

        std::string reference = readFile("single.entity");

        CHECK_EQUAL(reference, json);
    }


    TEST_FIXTURE(EntitiesTestEnvironment, SerializationToStringWithChildren)
    {
        Entity* pParent = pScene->create("parent");
        Entity* pChild = pScene->create("child");

        pParent->addChild(pChild);

        std::string json = toJSON(pParent);

        std::string reference = readFile("one_child.entity");

        CHECK_EQUAL(reference, json);
    }
}


SUITE(EntityJSONDeserialization)
{
    TEST_FIXTURE(EntitiesTestEnvironment, DeserializationFromObject)
    {
        rapidjson::Document document;
        rapidjson::Value components;
        rapidjson::Value component;
        rapidjson::Value properties;
        rapidjson::Value entry;
        rapidjson::Value field;


        document.SetObject();

        field.SetString("test");
        document.AddMember("name", field, document.GetAllocator());

        field.SetBool(true);
        document.AddMember("enabled", field, document.GetAllocator());


        components.SetArray();

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

        components.PushBack(component, document.GetAllocator());

        document.AddMember("components", components, document.GetAllocator());


        Entity* pEntity = fromJSON(document, pScene);
        CHECK(pEntity);
        CHECK_EQUAL("test", pEntity->getName());
        CHECK_EQUAL(1, pEntity->getNbComponents());
        CHECK_EQUAL(0, pEntity->getNbChildren());
    }


    TEST_FIXTURE(EntitiesTestEnvironment, DeserializationFromObjectWithChildren)
    {
        rapidjson::Document document;
        rapidjson::Value components;
        rapidjson::Value component;
        rapidjson::Value properties;
        rapidjson::Value children;
        rapidjson::Value child;
        rapidjson::Value entry;
        rapidjson::Value field;


        document.SetObject();

        field.SetString("parent");
        document.AddMember("name", field, document.GetAllocator());

        field.SetBool(true);
        document.AddMember("enabled", field, document.GetAllocator());


        components.SetArray();

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

        components.PushBack(component, document.GetAllocator());

        document.AddMember("components", components, document.GetAllocator());


        children.SetArray();

        child.SetObject();

        field.SetString("child");
        child.AddMember("name", field, document.GetAllocator());

        field.SetBool(true);
        child.AddMember("enabled", field, document.GetAllocator());

        components.SetArray();

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

        components.PushBack(component, document.GetAllocator());

        child.AddMember("components", components, document.GetAllocator());

        children.PushBack(child, document.GetAllocator());

        document.AddMember("children", children, document.GetAllocator());


        Entity* pParent = fromJSON(document, pScene);
        CHECK(pParent);
        CHECK_EQUAL("parent", pParent->getName());
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
