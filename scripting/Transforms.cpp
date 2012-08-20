#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Math/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace Athena::Math;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Transforms>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Transforms_New(const Arguments& args)
{
    // New C++ transforms
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new Transforms(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ transforms
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Transforms* pTransforms = static_cast<Transforms*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pTransforms, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nTransforms(name, components_list)\nTransforms(<C++ component>)"));
}


/************************************** PROPERTIES *************************************/

Handle<Value> Transforms_GetPosition(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getPosition()));
}

//-----------------------------------------------------------------------

void Transforms_SetPosition(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    if (!isJSVector3(value))
    {
        ThrowException(String::New("Invalid value, expected a Vector3"));
        return;
    }

    ptr->setPosition(fromJSVector3Unsafe(value));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetWorldPosition(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getWorldPosition()));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetOrientation(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getOrientation()));
}

//-----------------------------------------------------------------------

void Transforms_SetOrientation(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setOrientation(fromJSQuaternion(value));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetWorldOrientation(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getWorldOrientation()));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetInheritOrientation(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->inheritOrientation()));
}

//-----------------------------------------------------------------------

void Transforms_SetInheritOrientation(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setInheritOrientation(value->BooleanValue());
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetScale(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getScale()));
}

//-----------------------------------------------------------------------

void Transforms_SetScale(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    if (!isJSVector3(value))
    {
        ThrowException(String::New("Invalid value, expected a Vector3"));
        return;
    }

    ptr->setScale(fromJSVector3Unsafe(value));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetWorldScale(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getWorldScale()));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_GetInheritScale(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->inheritScale()));
}

//-----------------------------------------------------------------------

void Transforms_SetInheritScale(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setInheritScale(value->BooleanValue());
}


/**************************************** METHODS ***************************************/

Handle<Value> Transforms_Translate(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() == 1) || (args.Length() == 2)) && isJSVector3(args[0]))
    {
        Vector3 d = fromJSVector3Unsafe(args[0]);

        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        ptr->translate(d, relativeTo);

        return Handle<Value>();
    }
    else if (((args.Length() == 3) || (args.Length() == 4)) &&
             args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber())
    {
        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 4) && args[3]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[3]->Uint32Value();

        ptr->translate(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), relativeTo);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: translate(vector3 [, transform_space]) or translate(dx, dy, dz [, transform_space])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_SetDirection(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() >= 1) && (args.Length() <= 3)) && isJSVector3(args[0]))
    {
        Vector3 vec = fromJSVector3Unsafe(args[0]);

        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() >= 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        Vector3 localDirectionVector = Vector3::NEGATIVE_UNIT_Z;
        if ((args.Length() == 3) && isJSVector3(args[2]))
            localDirectionVector = fromJSVector3Unsafe(args[2]);

        ptr->setDirection(vec, relativeTo, localDirectionVector);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: setDirection(direction [, relativeTo [, localDirectionVector]])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_LookAt(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() >= 1) && (args.Length() <= 3)) && isJSVector3(args[0]))
    {
        Vector3 targetPoint = fromJSVector3Unsafe(args[0]);

        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() >= 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        Vector3 localDirectionVector = Vector3::NEGATIVE_UNIT_Z;
        if ((args.Length() == 3) && isJSVector3(args[2]))
            localDirectionVector = fromJSVector3Unsafe(args[2]);

        ptr->lookAt(targetPoint, relativeTo, localDirectionVector);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: lookAt(targetPoint [, relativeTo [, localDirectionVector]])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_Roll(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() >= 1) && (args.Length() <= 2)) && args[0]->IsNumber())
    {
        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        ptr->roll(Radian(args[0]->NumberValue()), relativeTo);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: roll(angle [, relativeTo])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_Pitch(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() >= 1) && (args.Length() <= 2)) && args[0]->IsNumber())
    {
        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        ptr->pitch(Radian(args[0]->NumberValue()), relativeTo);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: pitch(angle [, relativeTo])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_Yaw(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (((args.Length() >= 1) && (args.Length() <= 2)) && args[0]->IsNumber())
    {
        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        ptr->yaw(Radian(args[0]->NumberValue()), relativeTo);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: yaw(angle [, relativeTo])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_Rotate(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if (isJSVector3(args[0]))
    {
        if (args[1]->IsNumber())
        {
            Vector3 axis = fromJSVector3Unsafe(args[0]);

            Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
            if ((args.Length() == 3) && args[2]->IsUint32())
                relativeTo = (Transforms::tTransformSpace) args[2]->Uint32Value();

            ptr->rotate(axis, Radian(args[1]->NumberValue()), relativeTo);

            return Handle<Value>();
        }
    }
    else if (isJSQuaternion(args[0]))
    {
        Quaternion q = fromJSQuaternionUnsafe(args[0]);

        Transforms::tTransformSpace relativeTo = Transforms::TS_LOCAL;
        if ((args.Length() == 2) && args[1]->IsUint32())
            relativeTo = (Transforms::tTransformSpace) args[1]->Uint32Value();

        ptr->rotate(q, relativeTo);

        return Handle<Value>();
    }

    return ThrowException(String::New("Invalid parameters, syntax: rotate(axis, angle [, relativeTo]) or rotate(quaternion [, relativeTo])"));
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_ResetOrientation(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->resetOrientation();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Transforms_Rescale(const Arguments& args)
{
    HandleScope handle_scope;

    Transforms* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && isJSVector3(args[0]))
    {
        ptr->scale(fromJSVector3Unsafe(args[0]));
    }
    else if ((args.Length() == 3) &&
             args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber())
    {
        ptr->scale(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, syntax: scale(vector3) or scale(dx, dy, dz)"));
    }

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Transforms(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> transforms = pManager->getClassTemplate("Athena.Entities.Transforms");

    if (transforms.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Entities.Component").IsEmpty());

        // Declaration of the class
        transforms = FunctionTemplate::New(Transforms_New);
        transforms->InstanceTemplate()->SetInternalFieldCount(1);
        transforms->Inherit(pManager->getClassTemplate("Athena.Entities.Component"));

        // Attributes
        AddAttribute(transforms, "position",           Transforms_GetPosition,           Transforms_SetPosition);
        AddAttribute(transforms, "worldPosition",      Transforms_GetWorldPosition,      0);
        AddAttribute(transforms, "orientation",        Transforms_GetOrientation,        Transforms_SetOrientation);
        AddAttribute(transforms, "worldOrientation",   Transforms_GetWorldOrientation,   0);
        AddAttribute(transforms, "inheritOrientation", Transforms_GetInheritOrientation, Transforms_SetInheritOrientation);
        AddAttribute(transforms, "scale",              Transforms_GetScale,              Transforms_SetScale);
        AddAttribute(transforms, "worldScale",         Transforms_GetWorldScale,         0);
        AddAttribute(transforms, "inheritScale",       Transforms_GetInheritScale,       Transforms_SetInheritScale);

        // Methods
        AddMethod(transforms, "translate",        Transforms_Translate);
        AddMethod(transforms, "setDirection",     Transforms_SetDirection);
        AddMethod(transforms, "lookAt",           Transforms_LookAt);
        AddMethod(transforms, "roll",             Transforms_Roll);
        AddMethod(transforms, "pitch",            Transforms_Pitch);
        AddMethod(transforms, "yaw",              Transforms_Yaw);
        AddMethod(transforms, "rotate",           Transforms_Rotate);
        AddMethod(transforms, "resetOrientation", Transforms_ResetOrientation);
        AddMethod(transforms, "rescale",          Transforms_Rescale);

        pManager->declareClassTemplate("Athena.Entities.Transforms", transforms);

        parent->Set(String::New("Transforms_TYPE"), String::New(Athena::Entities::Transforms::TYPE.c_str()));

        parent->Set(String::New("TS_LOCAL"),  Uint32::New(Transforms::TS_LOCAL));
        parent->Set(String::New("TS_PARENT"), Uint32::New(Transforms::TS_PARENT));
        parent->Set(String::New("TS_WORLD"),  Uint32::New(Transforms::TS_WORLD));
    }

    // Add the class to the parent
    return parent->Set(String::New("Transforms"), transforms->GetFunction());
}
