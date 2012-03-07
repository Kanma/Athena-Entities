/** @file   Scripting.h
    @author Philip Abbet

    Scripting-related declarations for the Athena-Entities module
*/

#ifndef _ATHENA_ENTITIES_SCRIPTING_H_
#define _ATHENA_ENTITIES_SCRIPTING_H_

#include <v8.h>
#include <Athena-Entities/tComponentID.h>


namespace Athena {
namespace Entities {

    //------------------------------------------------------------------------------------
    /// @brief  Create a C++ ComponentID from a JavaScript one
    //------------------------------------------------------------------------------------
    tComponentID ATHENA_ENTITIES_SYMBOL fromJSComponentID(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a JavaScript ComponentID from a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(const tComponentID& value);
}
}

#endif
