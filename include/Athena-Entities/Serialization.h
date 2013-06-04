/** @file   Serialization.h
    @author Philip Abbet

    Serialization / Deserialization functions
*/

#ifndef _ATHENA_ENTITIES_SERIALIZATION_H_
#define _ATHENA_ENTITIES_SERIALIZATION_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>
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
    /// @param  pList               The list to which add the component
    /// @retval pDelayedProperties  If provided, the properties that aren't usable yet
    ///                             (because, for example, another object which isn't
    ///                             already created is needed) are put into that list by
    ///                             the describable
    /// @return                     The component, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Component* fromJSON(const rapidjson::Value& json_component,
                                                         Entities::ComponentsList* pList,
                                                         Utils::PropertiesList* pDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of a component as a string
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL std::string toJSON(Entities::Component* pComponent);


    //------------------------------------------------------------------------------------
    /// @brief Returns the component represented by a JSON string
    ///
    /// @param  json_component      The JSON string
    /// @param  pList               The list to which add the component
    /// @retval pDelayedProperties  If provided, the properties that aren't usable yet
    ///                             (because, for example, another object which isn't
    ///                             already created is needed) are put into that list by
    ///                             the component
    /// @return                     The component, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Component* fromJSON(const std::string& json_component,
                                                         Entities::ComponentsList* pList,
                                                         Utils::PropertiesList* pDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the rapidjson representation of an entity
    ///
    /// @param  pEntity         The entity
    /// @retval json_entity     The resulting rapidjson representation
    /// @param  allocator       The memory allocator to use
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL void toJSON(Entities::Entity* pEntity,
                                       rapidjson::Value &json_entity,
                                       rapidjson::Value::AllocatorType &allocator);


    //------------------------------------------------------------------------------------
    /// @brief Returns the entity represented by a rapidjson value
    ///
    /// @param  json_entity                 The rapidjson value
    /// @param  pScene                      The scene to use to create the entity
    /// @retval pCombinedDelayedProperties  If provided, the properties that aren't usable
    ///                                     yet (on this entity or any of its children)
    ///                                     are put into that list. The name of the
    ///                                     categories is of the form
    ///                                     <componentID>#<category>.
    /// @return                             The entity, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Entity* fromJSON(const rapidjson::Value& json_entity,
                                                      Entities::Scene* pScene,
                                                      Utils::PropertiesList* pCombinedDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of an entity as a string
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL std::string toJSON(Entities::Entity* pEntity);


    //------------------------------------------------------------------------------------
    /// @brief Returns the entity represented by a JSON string
    ///
    /// @param  json_entity                 The JSON string
    /// @param  pScene                      The scene to use to create the entity
    /// @retval pCombinedDelayedProperties  If provided, the properties that aren't usable
    ///                                     yet (on this entity or any of its children)
    ///                                     are put into that list. The name of the
    ///                                     categories is of the form
    ///                                     <componentID>#<category>.
    /// @return                             The entity, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Entity* fromJSON(const std::string& json_entity,
                                                      Entities::Scene* pScene,
                                                      Utils::PropertiesList* pCombinedDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the rapidjson representation of a scene
    ///
    /// @param  pScene          The scene
    /// @retval json_scene      The resulting rapidjson representation
    /// @param  allocator       The memory allocator to use
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL void toJSON(Entities::Scene* pScene,
                                       rapidjson::Value &json_scene,
                                       rapidjson::Value::AllocatorType &allocator);


    //------------------------------------------------------------------------------------
    /// @brief Returns the entity represented by a rapidjson value
    ///
    /// @param  json_scene                  The rapidjson value
    /// @retval pCombinedDelayedProperties  If provided, the properties that aren't usable
    ///                                     yet (on the scene or any of its entities)
    ///                                     are put into that list. The name of the
    ///                                     categories is of the form
    ///                                     <componentID>#<category>.
    /// @return                             The scene, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Scene* fromJSON(const rapidjson::Value& json_scene,
                                                     Utils::PropertiesList* pCombinedDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of a scene as a string
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL std::string toJSON(Entities::Scene* pScene);


    //------------------------------------------------------------------------------------
    /// @brief Returns the scene represented by a JSON string
    ///
    /// @param  json_scene                  The JSON string
    /// @retval pCombinedDelayedProperties  If provided, the properties that aren't usable
    ///                                     yet (on this entity or any of its children)
    ///                                     are put into that list. The name of the
    ///                                     categories is of the form
    ///                                     <componentID>#<category>.
    /// @return                             The scene, or 0 in case or failure
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entities::Scene* fromJSON(const std::string& json_scene,
                                                     Utils::PropertiesList* pCombinedDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Writes the JSON representation of a scene in a DataStream object
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL void toJSON(Entities::Scene* pScene, Data::DataStream* pStream);
}
}

#endif
