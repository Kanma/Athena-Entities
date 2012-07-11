#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<ScenesManager>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> ScenesManager_New(const Arguments& args)
{
    // Return a wrapper around the existing singleton object
    return SetObjectPtr(args.This(), ScenesManager::getSingletonPtr(), &NoOpWeakCallback);
}


/************************************** PROPERTIES *************************************/

Handle<Value> ScenesManager_GetNbScenes(Local<String> property, const AccessorInfo &info)
{
    ScenesManager* ptr = GetPtr(info.This());
    assert(ptr);

    return Uint32::New(ptr->getNbScenes());
}

//-----------------------------------------------------------------------

Handle<Value> ScenesManager_GetCurrentScene(Local<String> property, const AccessorInfo &info)
{
    ScenesManager* ptr = GetPtr(info.This());
    assert(ptr);

    return toJavaScript(ptr->getCurrentScene());
}


/**************************************** METHODS ***************************************/

Handle<Value> ScenesManager_Create(const Arguments& args)
{
    HandleScope handle_scope;

    ScenesManager* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsString())
    {
        return handle_scope.Close(toJavaScript(ptr->create(*String::AsciiValue(args[0]->ToString()))));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreate(name)"));
}

//-----------------------------------------------------------------------

Handle<Value> ScenesManager_Destroy(const Arguments& args)
{
    HandleScope handle_scope;

    ScenesManager* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->destroy(fromJSScene(args[0]));
    else if ((args.Length() == 1) && args[0]->IsString())
        ptr->destroy(*String::AsciiValue(args[0]->ToString()));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndestroy(scene)\ndestroy(name)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> ScenesManager_DestroyAll(const Arguments& args)
{
    HandleScope handle_scope;

    ScenesManager* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->destroyAll();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> ScenesManager_GetScene(const Arguments& args)
{
    HandleScope handle_scope;

    ScenesManager* ptr = GetPtr(args.This());
    assert(ptr);

    Scene* pScene = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
        pScene = ptr->getScene(args[0]->ToUint32()->Value());
    else if ((args.Length() == 1) && args[0]->IsString())
        pScene = ptr->getScene(*String::AsciiValue(args[0]->ToString()));

    if (!pScene)
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetScene(name)\ngetScene(index)"));

    return handle_scope.Close(toJavaScript(pScene));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_ScenesManager(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> scenes_manager = pManager->getClassTemplate("Athena.Entities.ScenesManager");

    if (scenes_manager.IsEmpty())
    {
        // Declaration of the class
        scenes_manager = FunctionTemplate::New(ScenesManager_New);
        scenes_manager->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(scenes_manager, "nbScenes",     ScenesManager_GetNbScenes, 0);
        AddAttribute(scenes_manager, "currentScene", ScenesManager_GetCurrentScene, 0);

        // Methods
        AddMethod(scenes_manager, "create",     ScenesManager_Create);
        AddMethod(scenes_manager, "destroy",    ScenesManager_Destroy);
        AddMethod(scenes_manager, "destroyAll", ScenesManager_DestroyAll);
        AddMethod(scenes_manager, "getScene",   ScenesManager_GetScene);

        pManager->declareClassTemplate("Athena.Entities.ScenesManager", scenes_manager);
    }

    // Add the class to the parent
    return parent->Set(String::New("ScenesManager"), scenes_manager->GetFunction());
}
