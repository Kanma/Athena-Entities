/** @file   tComponentID.h
    @author Philip Abbet

    Definition of the type 'Athena::Entities::tComponentID'
*/

#ifndef _ATHENA_ENTITIES_TCOMPONENTID_H_
#define _ATHENA_ENTITIES_TCOMPONENTID_H_

#include <Athena-Entities/Prerequisites.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief  Enumeration of the possible types of components
//----------------------------------------------------------------------------------------
enum tComponentType
{
    COMP_NONE,          ///< Invalid component
    COMP_TRANSFORMS,    ///< Transforms component
    COMP_VISUAL,        ///< Visual component
    COMP_AUDIO,         ///< Audio component
    COMP_PHYSICAL,      ///< Physical component
    COMP_DEBUG,         ///< Debug component
    COMP_OTHER,         ///< A component that doesn't fit into the other categories
};


//----------------------------------------------------------------------------------------
/// @brief  Represents an ID identifying a component of an entity
//----------------------------------------------------------------------------------------
struct ATHENA_ENTITIES_SYMBOL tComponentID
{
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  theType             Type of component
    /// @param  strEntityName       Name of the entity containing the component
    /// @param  strComponentName    Name of the component
    //------------------------------------------------------------------------------------
    tComponentID(tComponentType theType, const std::string& strEntityName,
                 const std::string& strComponentName)
    : type(theType), strEntity(strEntityName), strName(strComponentName)
    {
    }

    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  theType             Type of component
    /// @param  strComponentName    Name of the component
    //------------------------------------------------------------------------------------
    tComponentID(tComponentType theType, const std::string& strComponentName)
    : type(theType), strEntity(""), strName(strComponentName)
    {
    }

    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  theType     Type of component
    //------------------------------------------------------------------------------------
    tComponentID(tComponentType theType)
    : type(theType), strEntity(""), strName("")
    {
    }

    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  strID       String representation of a component ID
    //------------------------------------------------------------------------------------
    tComponentID(const std::string& strID)
    : type(COMP_NONE), strEntity(""), strName("")
    {
        size_t index = strID.find("://");
        if (index != std::string::npos)
        {
            size_t index2 = strID.find(":", index + 3);

            if (index2 != std::string::npos)
            {
                strEntity = strID.substr(index + 3, index2 - (index + 3));
                strName = strID.substr(index2 + 1);
            }
            else
            {
                strName = strID.substr(index + 3);
            }

            if (strID.substr(0, index) == "Transforms")
                type = COMP_TRANSFORMS;
            else if (strID.substr(0, index) == "Visual")
                type = COMP_VISUAL;
            else if (strID.substr(0, index) == "Audio")
                type = COMP_AUDIO;
            else if (strID.substr(0, index) == "Physical")
                type = COMP_PHYSICAL;
            else if (strID.substr(0, index) == "Debug")
                type = COMP_DEBUG;
            else if (strID.substr(0, index) == "Other")
                type = COMP_OTHER;
        }
    }

    //------------------------------------------------------------------------------------
    /// @brief  Comparison operator
    //------------------------------------------------------------------------------------
    bool operator==(const tComponentID& id)
    {
        return (type == id.type) && (strEntity == id.strEntity) && (strName == id.strName);
    }

    //------------------------------------------------------------------------------------
    /// @brief  Comparison operator
    //------------------------------------------------------------------------------------
    bool operator==(const tComponentID& id) const
    {
        return (type == id.type) && (strEntity == id.strEntity) && (strName == id.strName);
    }

    //------------------------------------------------------------------------------------
    /// @brief  Returns the string representation of the component ID
    //------------------------------------------------------------------------------------
    std::string toString() const
    {
        assert(strEntity.find(":") == std::string::npos);

        std::string suffix = strName;
        if (!strEntity.empty())
            suffix = strEntity + ":" + suffix;

        if (type == COMP_TRANSFORMS)
            return "Transforms://" + suffix;
        else if (type == COMP_VISUAL)
            return "Visual://" + suffix;
        else if (type == COMP_AUDIO)
            return "Audio://" + suffix;
        else if (type == COMP_PHYSICAL)
            return "Physical://" + suffix;
        else if (type == COMP_DEBUG)
            return "Debug://" + suffix;
        else if (type == COMP_OTHER)
            return "Other://" + suffix;

        return "None";
    }


    tComponentType  type;       ///< Type of component
    std::string     strEntity;  ///< Name of the entity holding the component
    std::string     strName;    ///< Name of the component
};

}
}

#endif
