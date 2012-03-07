/** @file   Prerequisites.h
    @author Philip Abbet
    
    Declaration of the types of the Athena-Entities module
*/

#ifndef _ATHENA_ENTITIES_PREREQUISITES_H_
#define _ATHENA_ENTITIES_PREREQUISITES_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Entities/Config.h>


/// Used to export symbols from the library
#if (ATHENA_PLATFORM == ATHENA_PLATFORM_WIN32) && !ATHENA_ENTITIES_STATIC
#    ifdef ATHENA_ENTITIES_EXPORTS
#        define ATHENA_ENTITIES_SYMBOL  __declspec(dllexport)
#    else
#        define ATHENA_ENTITIES_SYMBOL  __declspec(dllimport)
#    endif
#else
#    define ATHENA_ENTITIES_SYMBOL
#endif


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
        class Animation;
        class AnimationsMixer;
        class Component;
        class ComponentAnimation;
        class ComponentsList;
        class ComponentsManager;
        class Entity;
        class Scene;
        class ScenesManager;
        class Transforms;
        
        typedef unsigned int tAnimation;

        ATHENA_ENTITIES_SYMBOL extern const char* VERSION;
    }
}

#endif
