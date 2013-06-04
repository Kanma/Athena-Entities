/** @file   Serialization.cpp
    @author Philip Abbet

    Implementation of the serialization / deserialization functions
*/

#include <Athena-Entities/Serialization.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scene.h>
#include <Athena-Core/Data/Serialization.h>
#include <Athena-Core/Data/DataStream.h>
#include <Athena-Core/Log/LogManager.h>
#include <Athena-Core/Utils/StringUtils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


using namespace Athena::Entities;
using namespace Athena::Data;
using namespace Athena::Utils;
using namespace Athena::Log;
using namespace rapidjson;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Serialization";


/********************************** PRIVATE FUNCTIONS ***********************************/

namespace Athena {
namespace Entities {

void processCombinedDelayedProperties(Entities::Scene* pScene, PropertiesList* pDelayedProperties)
{
    PropertiesList::tCategoriesIterator categIter = pDelayedProperties->getCategoriesIterator();
    while (categIter.hasMoreElements())
    {
        PropertiesList::tCategory* pCategory = categIter.peekNextPtr();
        categIter.moveNext();

        StringUtils::tStringsList parts = StringUtils::split(pCategory->strName, "#");

        tComponentID id(parts[0]);

        Entity* pEntity = pScene->getEntity(id.strEntity);
        if (!pEntity)
            continue;

        Component* pComponent = pEntity->getComponent(id);
        if (!pComponent)
            continue;

        PropertiesList::tPropertiesList::iterator propIter, current, propIterEnd;
        for (propIter = pCategory->values.begin(), propIterEnd = pCategory->values.end(); propIter != propIterEnd; )
        {
            current = propIter;
            ++propIter;

            bool bUsed = pComponent->setProperty(parts[1], current->strName, new Variant(*(current->pValue)));
            if (bUsed)
            {
                delete current->pValue;
                pCategory->values.erase(current);
            }
        }
    }

    pDelayedProperties->removeEmptyCategories();
}

}
}


/************************************** FUNCTIONS ***************************************/

void Athena::Entities::toJSON(Entities::Component* pComponent,
                              rapidjson::Value &json_component,
                              rapidjson::Value::AllocatorType &allocator)
{
    // Assertions
    assert(pComponent);

    // Create the JSON representation
    json_component.SetObject();

    tComponentID id = pComponent->getID();
    id.strEntity = "";

    Value json_id;
    json_id.SetString(id.toString().c_str(), allocator);

    json_component.AddMember("id", json_id, allocator);

    Value json_properties;
    Data::toJSON(pComponent, json_properties, allocator);
    json_component.AddMember("properties", json_properties, allocator);
}

//-----------------------------------------------------------------------

Athena::Entities::Component* Athena::Entities::fromJSON(
                                                const rapidjson::Value& json_component,
                                                Entities::ComponentsList* pList,
                                                PropertiesList* pDelayedProperties)
{
    // Assertions
    assert(pList);
    assert(ComponentsManager::getSingletonPtr());

    // Check that the json representation contains all the required infos
    if (!json_component.IsObject())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Component: not an object");
        return 0;
    }

    if (!json_component.HasMember("id") || !json_component["id"].IsString())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Component: no component ID found");
        return 0;
    }

    if (!json_component.HasMember("properties") || !json_component["properties"].IsArray() ||
        (json_component["properties"].Size() == 0))
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Component: no properties found");
        return 0;
    }

    // Create the component
    const rapidjson::Value& properties = json_component["properties"];
    rapidjson::Value::ConstValueIterator  iter = properties.Begin();
    rapidjson::Value::ConstValueIterator  iterEnd = properties.End();

    ComponentsManager* pManager = ComponentsManager::getSingletonPtr();
    Component* pComponent = 0;

    tComponentID id(json_component["id"].GetString());

    pComponent = pList->getComponent(id);

    if (!pComponent)
    {
        ATHENA_LOG_COMMENT("Attempt to create a component with name: " + id.strName);

        while (!pComponent && (iter != iterEnd))
        {
            std::string strType = (*iter)["__category__"].GetString();
            ATHENA_LOG_COMMENT("  Trying with type: " + strType);

            pComponent = pManager->create(strType, id.strName, pList);

            ++iter;
        }
    }

    // Assign the properties to the component
    if (pComponent)
        Data::fromJSON(properties, pComponent, pDelayedProperties);

    // Return the component
    return pComponent;
}

//-----------------------------------------------------------------------

std::string Athena::Entities::toJSON(Entities::Component* pComponent)
{
    // Assertions
    assert(pComponent);

    // Retrieve the JSON representation
    Document document;
    toJSON(pComponent, document, document.GetAllocator());

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    return s.GetString();
}

//-----------------------------------------------------------------------

Athena::Entities::Component* Athena::Entities::fromJSON(const std::string& json_component,
                                                        Entities::ComponentsList* pList,
                                                        PropertiesList* pDelayedProperties)
{
    // Assertions
    assert(pList);

    // Convert to a JSON representation
    Document document;
    if (document.Parse<0>(json_component.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return 0;
    }

    return fromJSON(document, pList, pDelayedProperties);
}

//-----------------------------------------------------------------------

void Athena::Entities::toJSON(Entities::Entity* pEntity,
                              rapidjson::Value &json_entity,
                              rapidjson::Value::AllocatorType &allocator)
{
    // Assertions
    assert(pEntity);

    // Declarations
    Value value;
    Value array;

    // Create the JSON representation
    json_entity.SetObject();

    value.SetString(pEntity->getName().c_str(), allocator);
    json_entity.AddMember("name", value, allocator);

    value.SetBool(pEntity->isEnabled());
    json_entity.AddMember("enabled", value, allocator);

    // Components
    array.SetArray();

    Component::tComponentsIterator compIter = pEntity->getComponentsIterator();
    while (compIter.hasMoreElements())
    {
        Component* pComponent = compIter.getNext();
        toJSON(pComponent, value, allocator);
        array.PushBack(value, allocator);
    }

    json_entity.AddMember("components", array, allocator);

    // Children
    array.SetArray();

    Entity::tEntitiesIterator childrenIter = pEntity->getChildrenIterator();
    while (childrenIter.hasMoreElements())
    {
        Entity* pChild = childrenIter.getNext();
        toJSON(pChild, value, allocator);
        array.PushBack(value, allocator);
    }

    json_entity.AddMember("children", array, allocator);
}

//-----------------------------------------------------------------------

Athena::Entities::Entity* Athena::Entities::fromJSON(const rapidjson::Value& json_entity,
                                                     Entities::Scene* pScene,
                                                     Utils::PropertiesList* pCombinedDelayedProperties)
{
    // Assertions
    assert(pScene);

    // Check that the json representation contains all the required infos
    if (!json_entity.IsObject())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Entity: not an object");
        return 0;
    }

    if (!json_entity.HasMember("name") || !json_entity["name"].IsString())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Entity: no name found");
        return 0;
    }

    if (!json_entity.HasMember("components") || !json_entity["components"].IsArray() ||
        (json_entity["components"].Size() == 0))
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Entity: no components found");
        return 0;
    }

    // Create the entity
    Entity* pEntity = pScene->create(json_entity["name"].GetString());
    if (!pEntity)
        return 0;

    // Create the components
    const rapidjson::Value& components = json_entity["components"];
    rapidjson::Value::ConstValueIterator iter, iterEnd;

    PropertiesList delayedProperties;
    for (iter = components.Begin(), iterEnd = components.End(); iter != iterEnd; ++iter)
    {
        PropertiesList componentDelayedProperties;
        Component* pComponent = fromJSON(*iter, pEntity->getComponentsList(), &componentDelayedProperties);

        PropertiesList::tCategoriesIterator categIter = componentDelayedProperties.getCategoriesIterator();
        while (categIter.hasMoreElements())
        {
            PropertiesList::tCategory* pCategory = categIter.peekNextPtr();
            pCategory->strName = pComponent->getID().toString() + "#" + pCategory->strName;
            categIter.moveNext();
        }

        delayedProperties.append(&componentDelayedProperties);
    }

    // Create the children
    if (json_entity.HasMember("children"))
    {
        const rapidjson::Value& children = json_entity["children"];

        PropertiesList childDelayedProperties;

        for (iter = children.Begin(), iterEnd = children.End(); iter != iterEnd; ++iter)
        {
            Entity* pChild = fromJSON(*iter, pScene, &childDelayedProperties);
            if (pChild)
                pEntity->addChild(pChild);
        }

        delayedProperties.append(&childDelayedProperties);
    }

    // Attempt to resolve the delayed properties
    processCombinedDelayedProperties(pScene, &delayedProperties);

    if (pCombinedDelayedProperties && (delayedProperties.nbCategories() > 0))
        pCombinedDelayedProperties->append(&delayedProperties);

    // Disable the entity if needed
    if (json_entity.HasMember("enabled") && json_entity["enabled"].IsBool() &&
        !json_entity["enabled"].GetBool())
    {
        pEntity->enable(false);
    }

    // Return the entity
    return pEntity;
}

//-----------------------------------------------------------------------

std::string Athena::Entities::toJSON(Entities::Entity* pEntity)
{
    // Assertions
    assert(pEntity);

    // Retrieve the JSON representation
    Document document;
    toJSON(pEntity, document, document.GetAllocator());

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    return s.GetString();
}

//-----------------------------------------------------------------------

Athena::Entities::Entity* Athena::Entities::fromJSON(const std::string& json_entity,
                                                     Entities::Scene* pScene,
                                                     Utils::PropertiesList* pCombinedDelayedProperties)
{
    // Assertions
    assert(pScene);

    // Convert to a JSON representation
    Document document;
    if (document.Parse<0>(json_entity.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return 0;
    }

    return fromJSON(document, pScene, pCombinedDelayedProperties);
}

//-----------------------------------------------------------------------

void Athena::Entities::toJSON(Entities::Scene* pScene,
                              rapidjson::Value &json_scene,
                              rapidjson::Value::AllocatorType &allocator)
{
    // Assertions
    assert(pScene);

    // Declarations
    Value value;
    Value array;

    // Create the JSON representation
    json_scene.SetObject();

    value.SetString(pScene->getName().c_str(), allocator);
    json_scene.AddMember("name", value, allocator);

    value.SetBool(pScene->isEnabled());
    json_scene.AddMember("enabled", value, allocator);

    // Components
    array.SetArray();

    Component::tComponentsIterator compIter = pScene->getComponentsIterator();
    while (compIter.hasMoreElements())
    {
        Component* pComponent = compIter.getNext();
        toJSON(pComponent, value, allocator);
        array.PushBack(value, allocator);
    }

    json_scene.AddMember("components", array, allocator);

    // Entities
    array.SetArray();

    Entity::tEntitiesIterator childrenIter = pScene->getEntitiesIterator();
    while (childrenIter.hasMoreElements())
    {
        Entity* pEntity = childrenIter.getNext();
        if (!pEntity->getParent())
        {
            toJSON(pEntity, value, allocator);
            array.PushBack(value, allocator);
        }
    }

    json_scene.AddMember("entities", array, allocator);
}

//-----------------------------------------------------------------------

Athena::Entities::Scene* Athena::Entities::fromJSON(const rapidjson::Value& json_scene,
                                                    Utils::PropertiesList* pCombinedDelayedProperties)
{
    // Check that the json representation contains all the required infos
    if (!json_scene.IsObject())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Scene: not an object");
        return 0;
    }

    if (!json_scene.HasMember("name") || !json_scene["name"].IsString())
    {
        ATHENA_LOG_ERROR("Failed to deserialize the Scene: no name found");
        return 0;
    }

    // Create the scene
    Scene* pScene = new Scene(json_scene["name"].GetString());
    if (!pScene)
        return 0;

    // Create the components
    const rapidjson::Value& components = json_scene["components"];
    rapidjson::Value::ConstValueIterator iter, iterEnd;

    PropertiesList delayedProperties;
    for (iter = components.Begin(), iterEnd = components.End(); iter != iterEnd; ++iter)
    {
        PropertiesList componentDelayedProperties;
        Component* pComponent = fromJSON(*iter, pScene->getComponentsList(), &componentDelayedProperties);

        PropertiesList::tCategoriesIterator categIter = componentDelayedProperties.getCategoriesIterator();
        while (categIter.hasMoreElements())
        {
            PropertiesList::tCategory* pCategory = categIter.peekNextPtr();
            pCategory->strName = pComponent->getID().toString() + "#" + pCategory->strName;
            categIter.moveNext();
        }

        delayedProperties.append(&componentDelayedProperties);
    }

    // Create the entities
    if (json_scene.HasMember("entities"))
    {
        const rapidjson::Value& entities = json_scene["entities"];

        PropertiesList entityDelayedProperties;

        for (iter = entities.Begin(), iterEnd = entities.End(); iter != iterEnd; ++iter)
            fromJSON(*iter, pScene, &entityDelayedProperties);

        delayedProperties.append(&entityDelayedProperties);
    }

    // Attempt to resolve the delayed properties
    processCombinedDelayedProperties(pScene, &delayedProperties);

    if (pCombinedDelayedProperties && (delayedProperties.nbCategories() > 0))
        pCombinedDelayedProperties->append(&delayedProperties);

    // Disable the scene if needed
    if (json_scene.HasMember("enabled") && json_scene["enabled"].IsBool() &&
        !json_scene["enabled"].GetBool())
    {
        pScene->enable(false);
    }

    // Return the scene
    return pScene;
}

//-----------------------------------------------------------------------

std::string Athena::Entities::toJSON(Entities::Scene* pScene)
{
    // Assertions
    assert(pScene);

    // Retrieve the JSON representation
    Document document;
    toJSON(pScene, document, document.GetAllocator());

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    return s.GetString();
}

//-----------------------------------------------------------------------

Athena::Entities::Scene* Athena::Entities::fromJSON(const std::string& json_scene,
                                                    Utils::PropertiesList* pCombinedDelayedProperties)
{
    // Convert to a JSON representation
    Document document;
    if (document.Parse<0>(json_scene.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return 0;
    }

    return fromJSON(document, pCombinedDelayedProperties);
}

//-----------------------------------------------------------------------

void Athena::Entities::toJSON(Entities::Scene* pScene, Data::DataStream* pStream)
{
    // Assertions
    assert(pScene);
    assert(pStream);

    // Retrieve the JSON representation
    Document document;
    toJSON(pScene, document, document.GetAllocator());

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    pStream->write(s.GetString(), s.Size());
}

//-----------------------------------------------------------------------

Athena::Entities::Scene* Athena::Entities::fromJSON(Data::DataStream* pStream,
                                                    Utils::PropertiesList* pCombinedDelayedProperties)
{
    // Assertions
    assert(pStream);

    // Read the content of the file
    std::string content;
    char buffer[1025];

    while (!pStream->eof())
    {
        size_t count = pStream->read(buffer, 1024);
        buffer[count] = 0;
        content += buffer;
    }

    // Convert to a JSON representation
    Document document;
    if (document.Parse<0>(content.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return 0;
    }

    return fromJSON(document, pCombinedDelayedProperties);
}
