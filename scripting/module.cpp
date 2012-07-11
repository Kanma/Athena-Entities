/** @file   module.cpp
    @author Philip Abbet

    Entry point of the Javascript module of Athena::Entities
*/

#include <v8.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/tComponentID.h>
#include <string>

using namespace v8;
using namespace Athena::Scripting;
using namespace Athena::Entities;


/*********************************** EXTERNAL FUNCTIONS *********************************/

extern bool bind_ComponentsManager(Handle<Object> parent);
extern bool bind_ComponentsList(Handle<Object> parent);
extern bool bind_Component(Handle<Object> parent);
extern bool bind_Entity(Handle<Object> parent);
extern bool bind_ScenesManager(Handle<Object> parent);
extern bool bind_Scene(Handle<Object> parent);
extern bool bind_Transforms(Handle<Object> parent);


/*************************************** FUNCTIONS *************************************/

void bind_ComponentType(Handle<Object> parent)
{
    parent->Set(String::New("COMP_NONE"),       Uint32::New(Athena::Entities::COMP_NONE));
    parent->Set(String::New("COMP_TRANSFORMS"), Uint32::New(Athena::Entities::COMP_TRANSFORMS));
    parent->Set(String::New("COMP_VISUAL"),     Uint32::New(Athena::Entities::COMP_VISUAL));
    parent->Set(String::New("COMP_AUDIO"),      Uint32::New(Athena::Entities::COMP_AUDIO));
    parent->Set(String::New("COMP_PHYSICAL"),   Uint32::New(Athena::Entities::COMP_PHYSICAL));
    parent->Set(String::New("COMP_DEBUG"),      Uint32::New(Athena::Entities::COMP_DEBUG));
    parent->Set(String::New("COMP_OTHER"),      Uint32::New(Athena::Entities::COMP_OTHER));
}


bool load_js_file(const std::string& fileName, Handle<Object> parent, const std::string& modulePath)
{
    Handle<Value> result = ScriptingManager::getSingletonPtr()->executeFile(
                                        modulePath + "js/Entities/" + fileName + ".js",
                                        Context::GetCurrent());
    return !result.IsEmpty();
}


/****************************** INITIALISATION OF THE MODULE ****************************/

extern "C" {

    bool MODULE_INITIALISATION_FUNCTION init_module(Handle<Object> parent, const std::string& modulePath)
    {
        HandleScope handle_scope;

        if (parent->Get(String::New("Math"))->IsUndefined())
            ScriptingManager::getSingletonPtr()->import("Athena.Math", Context::GetCurrent());

        if (parent->Get(String::New("Core"))->IsUndefined())
            ScriptingManager::getSingletonPtr()->import("Athena.Core", Context::GetCurrent());

        parent->Set(String::New("VERSION"), String::New(Athena::Entities::VERSION));

        bind_ComponentType(parent);

        return load_js_file("ComponentID", parent, modulePath) &&
               bind_ComponentsManager(parent) &&
               bind_ComponentsList(parent) &&
               bind_Component(parent) &&
               bind_Entity(parent) &&
               bind_ScenesManager(parent) &&
               bind_Scene(parent) &&
               bind_Transforms(parent);
    }
}
