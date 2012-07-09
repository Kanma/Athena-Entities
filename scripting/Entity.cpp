#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scene.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Scripting/Conversions.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Entity>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Entity_New(const Arguments& args)
{
    // Wrapper around an existing C++ entity
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Entity* pEntity = static_cast<Entity*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pEntity, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nEntity(<C++ entity>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> Entity_GetName(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(String::New(ptr->getName().c_str()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetScene(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getScene()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetEnabled(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isEnabled()));
}

//-----------------------------------------------------------------------

void Entity_SetEnabled(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->enable(value->ToBoolean()->Value());
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetParent(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getParent()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetNbChildren(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbChildren()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetComponentsList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getComponentsList()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetNbComponents(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbComponents()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetSignalsList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getSignalsList()));
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetTransforms(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getTransforms()));
}


/**************************************** METHODS ***************************************/

Handle<Value> Entity_AddChild(const Arguments& args)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->addChild(fromJSEntity(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\naddChild(entity)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Entity_RemoveChild(const Arguments& args)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->removeChild(fromJSEntity(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\nremoveChild(entity)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Entity_DestroyAllChildren(const Arguments& args)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->destroyAllChildren();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Entity_GetComponent(const Arguments& args)
{
    HandleScope handle_scope;

    Entity* ptr = GetPtr(args.This());
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


/************************************ BINDING FUNCTION **********************************/

bool bind_Entity(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> entity = pManager->getClassTemplate("Athena.Entities.Entity");

    if (entity.IsEmpty())
    {
        // Declaration of the class
        entity = FunctionTemplate::New(Entity_New);
        entity->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(entity, "scene",           Entity_GetScene, 0);
        AddAttribute(entity, "name",            Entity_GetName, 0);
        AddAttribute(entity, "enabled",         Entity_GetEnabled, Entity_SetEnabled);
        AddAttribute(entity, "parent",          Entity_GetParent, 0);
        AddAttribute(entity, "nbChildren",      Entity_GetNbChildren, 0);
        AddAttribute(entity, "components",      Entity_GetComponentsList, 0);
        AddAttribute(entity, "nbComponents",    Entity_GetNbComponents, 0);
        AddAttribute(entity, "signals",         Entity_GetSignalsList, 0);
        AddAttribute(entity, "transforms",      Entity_GetTransforms, 0);

        // Methods
        AddMethod(entity, "addChild",           Entity_AddChild);
        AddMethod(entity, "removeChild",        Entity_RemoveChild);
        AddMethod(entity, "destroyAllChildren", Entity_DestroyAllChildren);
        AddMethod(entity, "getComponent",       Entity_GetComponent);

        pManager->declareClassTemplate("Athena.Entities.Entity", entity);
    }

    // Add the class to the parent
    return parent->Set(String::New("Entity"), entity->GetFunction());
}
