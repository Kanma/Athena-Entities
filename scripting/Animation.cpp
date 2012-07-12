#include <Athena-Entities/Animation.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Animation>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Animation_New(const Arguments& args)
{
    // Wrapper around an existing C++ animation
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Animation* pAnimation = static_cast<Animation*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pAnimation, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nAnimation(<C++ animation>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> Animation_GetName(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(String::New(ptr->getName().c_str()));
}

//-----------------------------------------------------------------------

Handle<Value> Animation_IsEnabled(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isEnabled()));
}

//-----------------------------------------------------------------------

void Animation_SetEnabled(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsBoolean())
    {
        ThrowException(String::New("'enabled' must be a boolean"));
        return;
    }

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setEnabled(value->BooleanValue());
}

//-----------------------------------------------------------------------

Handle<Value> Animation_GetWeight(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getWeight()));
}

//-----------------------------------------------------------------------

void Animation_SetWeight(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("'weight' must be a number"));
        return;
    }

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setWeight(value->NumberValue());
}

//-----------------------------------------------------------------------

Handle<Value> Animation_GetTimePosition(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getTimePosition()));
}

//-----------------------------------------------------------------------

void Animation_SetTimePosition(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsNumber())
    {
        ThrowException(String::New("'timePosition' must be a number"));
        return;
    }

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setTimePosition(value->NumberValue());
}

//-----------------------------------------------------------------------

Handle<Value> Animation_GetLength(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getLength()));
}

//-----------------------------------------------------------------------

Handle<Value> Animation_IsLooping(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isLooping()));
}

//-----------------------------------------------------------------------

void Animation_SetLooping(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    if (!value->IsBoolean())
    {
        ThrowException(String::New("'looping' must be a boolean"));
        return;
    }

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setLooping(value->BooleanValue());
}

//-----------------------------------------------------------------------

Handle<Value> Animation_GetNbComponentAnimations(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbComponentAnimations()));
}


/**************************************** METHODS ***************************************/

Handle<Value> Animation_Reset(const Arguments& args)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->reset();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Animation_Update(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsNumber())
    {
        Animation* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->update(args[0]->NumberValue());
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nupdate(elapsed_seconds)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Animation_AddComponentAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->addComponentAnimation(fromJSComponentAnimation(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\naddComponentAnimation(component_animation)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Animation_RemoveComponentAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->removeComponentAnimation(fromJSComponentAnimation(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\nremoveComponentAnimation(component_animation)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Animation_GetComponentAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    Animation* ptr = GetPtr(args.This());
    assert(ptr);

    ComponentAnimation* pComponentAnimation = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
        pComponentAnimation = ptr->getComponentAnimation(args[0]->ToUint32()->Value());

    if (!pComponentAnimation)
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetComponentAnimation(index)"));

    return handle_scope.Close(toJavaScript(pComponentAnimation));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Animation(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> animation = pManager->getClassTemplate("Athena.Entities.Animation");

    if (animation.IsEmpty())
    {
        // Declaration of the class
        animation = FunctionTemplate::New(Animation_New);
        animation->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(animation, "name",                  Animation_GetName, 0);
        AddAttribute(animation, "enabled",               Animation_IsEnabled, Animation_SetEnabled);
        AddAttribute(animation, "weight",                Animation_GetWeight, Animation_SetWeight);
        AddAttribute(animation, "timePosition",          Animation_GetTimePosition, Animation_SetTimePosition);
        AddAttribute(animation, "length",                Animation_GetLength, 0);
        AddAttribute(animation, "looping",               Animation_IsLooping, Animation_SetLooping);
        AddAttribute(animation, "nbComponentAnimations", Animation_GetNbComponentAnimations, 0);

        // Methods
        AddMethod(animation, "reset",                    Animation_Reset);
        AddMethod(animation, "update",                   Animation_Update);
        AddMethod(animation, "addComponentAnimation",    Animation_AddComponentAnimation);
        AddMethod(animation, "removeComponentAnimation", Animation_RemoveComponentAnimation);
        AddMethod(animation, "getComponentAnimation",    Animation_GetComponentAnimation);

        pManager->declareClassTemplate("Athena.Entities.Animation", animation);
    }

    // Add the class to the parent
    return parent->Set(String::New("Animation"), animation->GetFunction());
}
