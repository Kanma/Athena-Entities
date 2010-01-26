/** @file   Prerequisites.h
    @author Philip Abbet
    
    Declaration of the types of the Athena-Entities module
*/

#ifndef _ATHENA_ENTITIES_PREREQUISITES_H_
#define _ATHENA_ENTITIES_PREREQUISITES_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Entities/Config.h>


//----------------------------------------------------------------------------------------
/// @brief	Main namespace. All the components of the Athena engine belongs to this
///			namespace
//----------------------------------------------------------------------------------------
namespace Athena
{
	//------------------------------------------------------------------------------------
	/// @brief	Contains all the entities-related classes
	//------------------------------------------------------------------------------------
    namespace Entities
    {
        class Component;
        class ComponentsList;
        class ComponentsManager;
        class Entity;
        class Scene;
    }
}

#endif
