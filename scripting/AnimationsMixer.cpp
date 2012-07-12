#include <Athena-Entities/AnimationsMixer.h>
#include <Athena-Entities/Animation.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<AnimationsMixer>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> AnimationsMixer_New(const Arguments& args)
{
    // Wrapper around an existing C++ animations mixer
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        AnimationsMixer* pMixer = static_cast<AnimationsMixer*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pMixer, &NoOpWeakCallback);
    }

    // New  C++ animations mixer
    else if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new AnimationsMixer());
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nAnimationsMixer()\nAnimationsMixer(<C++ animations mixer>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> AnimationsMixer_IsCurrentAnimationDone(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isCurrentAnimationDone()));
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetCurrentAnimationTime(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getCurrentAnimationTime()));
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetCurrentAnimationLength(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getCurrentAnimationLength()));
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetCurrentAnimationName(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    Animation* pCurrentAnimation = ptr->getCurrentAnimation();

    if (pCurrentAnimation)
        return handle_scope.Close(String::New(pCurrentAnimation->getName().c_str()));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetCurrentAnimation(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getCurrentAnimation()));
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetNbAnimations(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getNbAnimations()));
}


/**************************************** METHODS ***************************************/

Handle<Value> AnimationsMixer_Update(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsNumber())
    {
        AnimationsMixer* ptr = GetPtr(args.This());
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

Handle<Value> AnimationsMixer_StartAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() == 1) || (args.Length() == 2)) && args[0]->IsString())
    {
        bool bReset = false;
        if ((args.Length() == 2) && args[0]->IsBoolean())
            bReset = args[0]->BooleanValue();

        ptr->startAnimation(*String::AsciiValue(args[0]->ToString()), bReset);
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nstartAnimation(name)\nstartAnimation(name, reset)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_StopAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(args.This());
    assert(ptr);

    if (args.Length() == 0)
        ptr->stopAnimation();
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\nstopAnimation()"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_CreateAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsString())
    {
        Animation* pAnimation = new Animation(*String::AsciiValue(args[0]->ToString()));
        ptr->addAnimation(pAnimation);

        return handle_scope.Close(toJavaScript(pAnimation));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateAnimation(name)"));
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_DestroyAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsString())
        ptr->removeAnimation(*String::AsciiValue(args[0]->ToString()));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndestroyAnimation(name)"));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> AnimationsMixer_GetAnimation(const Arguments& args)
{
    HandleScope handle_scope;

    AnimationsMixer* ptr = GetPtr(args.This());
    assert(ptr);

    Animation* pAnimation = 0;

    if ((args.Length() == 1) && args[0]->IsString())
        pAnimation = ptr->getAnimation(*String::AsciiValue(args[0]->ToString()));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetAnimation(name)"));

    return handle_scope.Close(toJavaScript(pAnimation));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_AnimationsMixer(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> mixer = pManager->getClassTemplate("Athena.Entities.AnimationsMixer");

    if (mixer.IsEmpty())
    {
        // Declaration of the class
        mixer = FunctionTemplate::New(AnimationsMixer_New);
        mixer->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(mixer, "currentAnimationDone",   AnimationsMixer_IsCurrentAnimationDone, 0);
        AddAttribute(mixer, "currentAnimationTime",   AnimationsMixer_GetCurrentAnimationTime, 0);
        AddAttribute(mixer, "currentAnimationLength", AnimationsMixer_GetCurrentAnimationLength, 0);
        AddAttribute(mixer, "currentAnimationName",   AnimationsMixer_GetCurrentAnimationName, 0);
        AddAttribute(mixer, "currentAnimation",       AnimationsMixer_GetCurrentAnimation, 0);
        AddAttribute(mixer, "nbAnimations",           AnimationsMixer_GetNbAnimations, 0);

        // Methods
        AddMethod(mixer, "update",                   AnimationsMixer_Update);
        AddMethod(mixer, "startAnimation",           AnimationsMixer_StartAnimation);
        AddMethod(mixer, "stopAnimation",            AnimationsMixer_StopAnimation);
        AddMethod(mixer, "createAnimation",          AnimationsMixer_CreateAnimation);
        AddMethod(mixer, "destroyAnimation",         AnimationsMixer_DestroyAnimation);
        AddMethod(mixer, "getAnimation",             AnimationsMixer_GetAnimation);

        pManager->declareClassTemplate("Athena.Entities.AnimationsMixer", mixer);
    }

    // Add the class to the parent
    return parent->Set(String::New("AnimationsMixer"), mixer->GetFunction());
}
