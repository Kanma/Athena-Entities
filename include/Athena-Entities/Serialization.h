/** @file   Serialization.h
    @author Philip Abbet

    Serialization / Deserialization functions
*/

#ifndef _ATHENA_ENTITIES_SERIALIZATION_H_
#define _ATHENA_ENTITIES_SERIALIZATION_H_

#include <Athena-Entities/Prerequisites.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>


namespace Athena {
namespace Entities {

    //------------------------------------------------------------------------------------
    /// @brief Returns the rapidjson representation of a component
    ///
    /// @param  pComponent      The component
    /// @retval json_component  The resulting rapidjson representation
    /// @param  allocator       The memory allocator to use
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL void toJSON(Entities::Component* pComponent,
                                       rapidjson::Value &json_component,
                                       rapidjson::Value::AllocatorType &allocator);


    //------------------------------------------------------------------------------------
    /// @brief Returns the component represented by a rapidjson value
    ///
    /// @param  json_component      The rapidjson value
    /// @retval pComponent          The component
    /// @retval pDelayedProperties  If provided, the properties that aren't usable yet
    ///                             (because, for example, another object which isn't
    ///                             already created is needed) are put into that list by
    ///                             the describable
    //------------------------------------------------------------------------------------
    // ATHENA_ENTITIES_SYMBOL void fromJSON(const rapidjson::Value& json_component,
    //                                      Entities::Component* pComponent,
    //                                      Utils::PropertiesList* pDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of a component as a string
    //------------------------------------------------------------------------------------
    // ATHENA_ENTITIES_SYMBOL std::string toJSON(Entities::Component* pComponent);


    //------------------------------------------------------------------------------------
    /// @brief Returns the component represented by a JSON string
    ///
    /// @param  json_component      The JSON string
    /// @retval pComponent          The component
    /// @retval pDelayedProperties  If provided, the properties that aren't usable yet
    ///                             (because, for example, another object which isn't
    ///                             already created is needed) are put into that list by
    ///                             the component
    //------------------------------------------------------------------------------------
    // ATHENA_ENTITIES_SYMBOL bool fromJSON(const std::string& json_component,
    //                                      Entities::Component* pComponent,
    //                                      Utils::PropertiesList* pDelayedProperties = 0);
}
}

#endif
