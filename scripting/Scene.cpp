#include <Athena-Entities/Scene.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Scripting/Conversions.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Scene>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Scene_New(const Arguments& args)
{
    // New C++ scene
    if ((args.Length() == 1) && args[0]->IsString())
    {
        return SetObjectPtr(args.This(), new Scene(*String::AsciiValue(args[0]->ToString())), &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ scene
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Scene* pScene = static_cast<Scene*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pScene, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nScene(name)\nScene(<C++ scene>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> Scene_GetName(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(String::New(ptr->getName().c_str()));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetEnabled(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isEnabled()));
}

//-----------------------------------------------------------------------

void Scene_SetEnabled(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->enable(value->ToBoolean()->Value());
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetShown(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isShown()));
}

//-----------------------------------------------------------------------

void Scene_SetShown(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    if (value->ToBoolean()->Value())
        ptr->show();
    else
        ptr->hide();
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetNbEntities(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbEntities()));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetComponentsList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getComponentsList()));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetNbComponents(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbComponents()));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetSignalsList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getSignalsList()));
}


/**************************************** METHODS ***************************************/

Handle<Value> Scene_CreateEntity(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() == 1) || (args.Length() == 2)) && args[0]->IsString())
    {
        Entity* pParent = 0;
        if (args.Length() == 2)
            GetObjectPtr(args[1], &pParent);

        return handle_scope.Close(toJavaScript(ptr->create(*String::AsciiValue(args[0]->ToString()), pParent)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreate(name [, parent])"));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetEntity(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    Entity* pEntity = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
        pEntity = ptr->getEntity(args[0]->ToUint32()->Value());
    else if ((args.Length() == 1) && args[0]->IsString())
        pEntity = ptr->getEntity(*String::AsciiValue(args[0]->ToString()));

    if (!pEntity)
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetEntity(name)\ngetEntity(index)"));

    return handle_scope.Close(toJavaScript(pEntity));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_DestroyEntity(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsString())
        ptr->destroy(*String::AsciiValue(args[0]->ToString()));
    else if ((args.Length() == 1) && args[0]->IsObject())
        ptr->destroy(fromJSEntity(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndestroy(name)\ndestroy(entity)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Scene_DestroyAll(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->destroyAll();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Scene_Transfer(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
        ptr->transfer(*String::AsciiValue(args[0]->ToString()), fromJSScene(args[1]));
    else if ((args.Length() == 1) && args[0]->IsObject())
        ptr->transfer(fromJSEntity(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ntransfer(entity)\ntransfer(name, scene)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetComponent(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    Component* pComponent = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
        pComponent = ptr->getComponent(args[0]->ToUint32()->Value());
    else if ((args.Length() == 1) && args[0]->IsObject())
        pComponent = ptr->getComponent(fromJSComponentID(args[0]));

    if (!pComponent)
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetComponent(id)\ngetComponent(index)"));

    return handle_scope.Close(toJavaScript(pComponent));
}

//-----------------------------------------------------------------------

Handle<Value> Scene_GetMainComponent(const Arguments& args)
{
    HandleScope handle_scope;

    Scene* ptr = GetPtr(args.This());
    assert(ptr);

    Component* pComponent = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
        pComponent = ptr->getMainComponent((tComponentType) args[0]->ToUint32()->Value());
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetMainComponent(type)"));

    return handle_scope.Close(toJavaScript(pComponent));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Scene(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> scene = pManager->getClassTemplate("Athena.Entities.Scene");

    if (scene.IsEmpty())
    {
        // Declaration of the class
        scene = FunctionTemplate::New(Scene_New);
        scene->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(scene, "name",         Scene_GetName, 0);
        AddAttribute(scene, "enabled",      Scene_GetEnabled, Scene_SetEnabled);
        AddAttribute(scene, "shown",        Scene_GetShown, Scene_SetShown);
        AddAttribute(scene, "nbEntities",   Scene_GetNbEntities, 0);
        AddAttribute(scene, "components",   Scene_GetComponentsList, 0);
        AddAttribute(scene, "nbComponents", Scene_GetNbComponents, 0);
        AddAttribute(scene, "signals",      Scene_GetSignalsList, 0);

        // Methods - Entities management
        AddMethod(scene, "create",           Scene_CreateEntity);
        AddMethod(scene, "getEntity",        Scene_GetEntity);
        AddMethod(scene, "destroy",          Scene_DestroyEntity);
        AddMethod(scene, "destroyAll",       Scene_DestroyAll);
        AddMethod(scene, "transfer",         Scene_Transfer);

        // Methods - Components management
        AddMethod(scene, "getComponent",     Scene_GetComponent);
        AddMethod(scene, "getMainComponent", Scene_GetMainComponent);

        pManager->declareClassTemplate("Athena.Entities.Scene", scene);
    }

    // Add the class to the parent
    return parent->Set(String::New("Scene"), scene->GetFunction());
}
