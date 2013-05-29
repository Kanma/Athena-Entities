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
#include <Athena-Core/Log/LogManager.h>
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
                                                     Entities::Scene* pScene)
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

    for (iter = components.Begin(), iterEnd = components.End(); iter != iterEnd; ++iter)
    {
        
        fromJSON(*iter, pEntity->getComponentsList());
    }

    // Create the children
    if (json_entity.HasMember("children"))
    {
        const rapidjson::Value& children = json_entity["children"];

        for (iter = children.Begin(), iterEnd = children.End(); iter != iterEnd; ++iter)
        {
            Entity* pChild = fromJSON(*iter, pScene);
            if (pChild)
                pEntity->addChild(pChild);
        }
    }

    // Disable the entity if needed
    if (json_entity.HasMember("enabled") && json_entity["enabled"].IsBool() &&
        !json_entity["enabled"].GetBool())
    {
        pEntity->enable(false);
    }

    // Return the component
    return pEntity;
}
