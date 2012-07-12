#include <Athena-Entities/ComponentAnimation.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<ComponentAnimation>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> ComponentAnimation_New(const Arguments& args)
{
    // Wrapper around an existing C++ component animation
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        ComponentAnimation* pAnimation = static_cast<ComponentAnimation*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pAnimation, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nComponentAnimation(<C++ component animation>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> ComponentAnimation_GetOffset(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getOffset()));
}

//-----------------------------------------------------------------------

void ComponentAnimation_SetOffset(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("offset must be a number"));
        return;
    }

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setOffset(value->NumberValue());
}

//-----------------------------------------------------------------------

Handle<Value> ComponentAnimation_GetWeight(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getWeight()));
}

//-----------------------------------------------------------------------

void ComponentAnimation_SetWeight(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("weight must be a number"));
        return;
    }

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setWeight(value->NumberValue());
}

//-----------------------------------------------------------------------

Handle<Value> ComponentAnimation_GetTimePosition(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getTimePosition()));
}

//-----------------------------------------------------------------------

void ComponentAnimation_SetTimePosition(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("timePosition must be a number"));
        return;
    }

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setTimePosition(value->NumberValue());
}

//-----------------------------------------------------------------------

Handle<Value> ComponentAnimation_GetLength(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getLength()));
}

//-----------------------------------------------------------------------

Handle<Value> ComponentAnimation_GetCurrentWeight(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getCurrentWeight()));
}

//-----------------------------------------------------------------------

void ComponentAnimation_SetCurrentWeight(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("currentWeight must be a number"));
        return;
    }

    ComponentAnimation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setCurrentWeight(value->NumberValue());
}


/**************************************** METHODS ***************************************/

Handle<Value> ComponentAnimation_Update(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsNumber())
    {
        ComponentAnimation* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->update(args[0]->NumberValue());
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nupdate(elapsed_seconds)"));
    }

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_ComponentAnimation(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> component_animation = pManager->getClassTemplate("Athena.Entities.ComponentAnimation");

    if (component_animation.IsEmpty())
    {
        // Declaration of the class
        component_animation = FunctionTemplate::New(ComponentAnimation_New);
        component_animation->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(component_animation, "offset",         ComponentAnimation_GetOffset, ComponentAnimation_SetOffset);
        AddAttribute(component_animation, "weight",         ComponentAnimation_GetWeight, ComponentAnimation_SetWeight);
        AddAttribute(component_animation, "timePosition",   ComponentAnimation_GetTimePosition, ComponentAnimation_SetTimePosition);
        AddAttribute(component_animation, "length",         ComponentAnimation_GetLength, 0);
        AddAttribute(component_animation, "currentWeight",  ComponentAnimation_GetCurrentWeight, ComponentAnimation_SetCurrentWeight);

        // Methods
        AddMethod(component_animation, "update", ComponentAnimation_Update);

        pManager->declareClassTemplate("Athena.Entities.ComponentAnimation", component_animation);
    }

    // Add the class to the parent
    return parent->Set(String::New("ComponentAnimation"), component_animation->GetFunction());
}
