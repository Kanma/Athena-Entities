/** @file   module.cpp
    @author Philip Abbet

    Entry point of the Javascript module of Athena::Entities
*/

#include <v8.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Entities/Prerequisites.h>
#include <string>

using namespace v8;
using namespace Athena::Entities;


/*********************************** EXTERNAL FUNCTIONS *********************************/


/****************************** INITIALISATION OF THE MODULE ****************************/

extern "C" {

    bool MODULE_INITIALISATION_FUNCTION init_module(Handle<Object> parent, const std::string& modulePath)
    {
        HandleScope handle_scope;

        parent->Set(String::New("VERSION"), String::New(Athena::Entities::VERSION));

        return true;
    }
}
