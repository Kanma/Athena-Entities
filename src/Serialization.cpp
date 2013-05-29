/** @file   Serialization.cpp
    @author Philip Abbet

    Implementation of the serialization / deserialization functions
*/

#include <Athena-Entities/Serialization.h>
#include <Athena-Entities/Component.h>
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
