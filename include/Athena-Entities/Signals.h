/**	@file	Signals.h
	@author	Philip Abbet

	Declaration of the signals of the Entities module
*/

#ifndef _ATHENA_ENTITIES_SIGNALS_H_
#define _ATHENA_ENTITIES_SIGNALS_H_

#include <Athena-Core/Signals/Declarations.h>


namespace Athena {
namespace Entities {

enum tSignals
{
	// Scenes
	SIGNAL_SCENE_ENABLED = Signals::SIGNALS_ENTITIES,   ///< Fired when a scene is enabled
	SIGNAL_SCENE_DISABLED,						        ///< Fired when a scene is disabled
	SIGNAL_SCENE_SHOWN,							        ///< Fired when a scene is shown
	SIGNAL_SCENE_HIDDEN,						        ///< Fired when a scene is hidden
                                                
	// Entities                                 
	SIGNAL_ENTITY_ENABLED,						        ///< Fired when an entity is enabled
	SIGNAL_ENTITY_DISABLED,						        ///< Fired when an entity is disabled
                                                
	// Components                               
	SIGNAL_COMPONENT_TRANSFORMS_CHANGED,		        ///< Fired when the transforms of a component have changed
	SIGNAL_COMPONENT_DESTROYED,					        ///< Fired when a component is destroyed
	SIGNAL_COMPONENT_PARENT_TRANSFORMS_CHANGED,	        ///< Fired when the parent transforms component of a component has changed
};

}
}

#endif
