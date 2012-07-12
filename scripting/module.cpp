/** @file   module.cpp
    @author Philip Abbet

    Entry point of the Javascript module of Athena::Entities
*/

#include <v8.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/tComponentID.h>
#include <Athena-Entities/Signals.h>
#include <string>

using namespace v8;
using namespace Athena::Scripting;
using namespace Athena::Entities;


/*********************************** EXTERNAL FUNCTIONS *********************************/

extern bool bind_Animation(Handle<Object> parent);
extern bool bind_AnimationsMixer(Handle<Object> parent);
extern bool bind_ComponentsManager(Handle<Object> parent);
extern bool bind_ComponentsList(Handle<Object> parent);
extern bool bind_Component(Handle<Object> parent);
extern bool bind_ComponentAnimation(Handle<Object> parent);
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


void bind_Signals(Handle<Object> parent)
{
    parent->Set(String::New("SIGNAL_SCENE_ENABLED"),       Uint32::New(Athena::Entities::SIGNAL_SCENE_ENABLED));
    parent->Set(String::New("SIGNAL_SCENE_DISABLED"),      Uint32::New(Athena::Entities::SIGNAL_SCENE_DISABLED));
    parent->Set(String::New("SIGNAL_SCENE_SHOWN"),         Uint32::New(Athena::Entities::SIGNAL_SCENE_SHOWN));
    parent->Set(String::New("SIGNAL_SCENE_HIDDEN"),        Uint32::New(Athena::Entities::SIGNAL_SCENE_HIDDEN));
    parent->Set(String::New("SIGNAL_ENTITY_ENABLED"),      Uint32::New(Athena::Entities::SIGNAL_ENTITY_ENABLED));
    parent->Set(String::New("SIGNAL_ENTITY_DISABLED"),     Uint32::New(Athena::Entities::SIGNAL_ENTITY_DISABLED));
    parent->Set(String::New("SIGNAL_COMPONENT_DESTROYED"), Uint32::New(Athena::Entities::SIGNAL_COMPONENT_DESTROYED));
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

        Handle<Context> context = parent->CreationContext();
        Handle<Object> ns = context->Global()->Get(String::New("Athena"))->ToObject();

        if (ns->Get(String::New("Math"))->IsUndefined())
            ScriptingManager::getSingletonPtr()->import("Athena.Math", Context::GetCurrent());

        if (ns->Get(String::New("Core"))->IsUndefined())
            ScriptingManager::getSingletonPtr()->import("Athena.Core", Context::GetCurrent());

        parent->Set(String::New("VERSION"), String::New(Athena::Entities::VERSION));

        bind_ComponentType(parent);
        bind_Signals(parent);

        return load_js_file("ComponentID", parent, modulePath) &&
               bind_Animation(parent) &&
               bind_AnimationsMixer(parent) &&
               bind_ComponentsManager(parent) &&
               bind_ComponentsList(parent) &&
               bind_Component(parent) &&
               bind_ComponentAnimation(parent) &&
               bind_Entity(parent) &&
               bind_ScenesManager(parent) &&
               bind_Scene(parent) &&
               bind_Transforms(parent);
    }
}
