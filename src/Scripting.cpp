/** @file   Scripting.cpp
    @author Philip Abbet

    Implementation of the scripting-related functions of the Athena-Entities module
*/

#include <Athena-Entities/Scripting.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Scripting/Utils.h>
#include <string>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


namespace Athena {
namespace Entities {


tComponentID fromJSComponentID(Handle<Value> value)
{
    if (value->IsObject())
    {
        Handle<Object> object = value->ToObject();
        Handle<Function> prototype = Handle<Function>::Cast(object->GetPrototype());

        if (std::string("Athena.Entities.ComponentID") == *String::AsciiValue(prototype->Get(String::New("__classname__"))))
        {
            return tComponentID((tComponentType) object->Get(String::New("type"))->ToNumber()->NumberValue(),
                                *String::AsciiValue(object->Get(String::New("entity"))),
                                *String::AsciiValue(object->Get(String::New("component"))));
        }
    }

    return tComponentID(COMP_NONE);
}

//-----------------------------------------------------------------------

Handle<Value> toJavaScript(const tComponentID& value)
{
    Local<Object> object = ScriptingManager::getSingletonPtr()->execute("new Athena.Entities.ComponentID('" + value.toString() + "');")->ToObject();

    // Handle<Value> constructor = Context::GetCurrent()->Global()->Get(String::New("Athena.Entities.ComponentID"));
    // if (!constructor->IsUndefined())
    //     return ThrowException(String::New("BLAAAAAhD"));
    // if (!constructor->IsFunction())
    //     return ThrowException(String::New("Can't find the constructor function of Athena.Entities.ComponentID"));

    // Local<Object> object = Handle<Function>::Cast(constructor)->NewInstance();

    // object->Set(String::New("type"),      Number::New(value.type));
    // object->Set(String::New("entity"),    String::New(value.strEntity.c_str()));
    // object->Set(String::New("component"), String::New(value.strName.c_str()));

    return object;
}

//-----------------------------------------------------------------------

ComponentsList* fromJSComponentsList(Handle<Value> value)
{
    if (value->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(value, &pList);
        return pList;
    }

    return 0;
}

//-----------------------------------------------------------------------

Handle<Object> createJSComponentsList()
{
    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.ComponentsList");

    Handle<Object> jsList = func->GetFunction()->NewInstance();

    return handle_scope.Close(jsList);
}

//-----------------------------------------------------------------------

Handle<Value> toJavaScript(ComponentsList* pList)
{
    // Assertions
    assert(pList);

    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.ComponentsList");

    Handle<Value> argv[1];
    argv[0] = External::Wrap(pList);

    Handle<Object> jsList = func->GetFunction()->NewInstance(1, argv);

    return handle_scope.Close(jsList);
}

//-----------------------------------------------------------------------

Component* fromJSComponent(Handle<Value> value)
{
    if (value->IsObject())
    {
        Component* pComponent = 0;
        GetObjectPtr(value, &pComponent);
        return pComponent;
    }

    return 0;
}

//-----------------------------------------------------------------------

Handle<Object> createJSComponent()
{
    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.Component");

    Handle<Object> jsComponent = func->GetFunction()->NewInstance();

    return handle_scope.Close(jsComponent);
}

//-----------------------------------------------------------------------

Handle<Value> toJavaScript(Component* pComponent)
{
    // Assertions
    assert(pComponent);

    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.Component");

    Handle<Value> argv[1];
    argv[0] = External::Wrap(pComponent);

    Handle<Object> jsComponent = func->GetFunction()->NewInstance(1, argv);

    return handle_scope.Close(jsComponent);
}

//-----------------------------------------------------------------------

Transforms* fromJSTransforms(Handle<Value> value)
{
    if (value->IsObject())
    {
        Transforms* pTransforms = 0;
        GetObjectPtr(value, &pTransforms);
        return pTransforms;
    }

    return 0;
}

//-----------------------------------------------------------------------

Handle<Object> createJSTransforms()
{
    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.Transforms");

    Handle<Object> jsTransforms = func->GetFunction()->NewInstance();

    return handle_scope.Close(jsTransforms);
}

//-----------------------------------------------------------------------

Handle<Value> toJavaScript(Transforms* pTransforms)
{
    // Assertions
    assert(pTransforms);

    HandleScope handle_scope;

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate(
                                                        "Athena.Entities.Transforms");

    Handle<Value> argv[1];
    argv[0] = External::Wrap(pTransforms);

    Handle<Object> jsTransforms = func->GetFunction()->NewInstance(1, argv);

    return handle_scope.Close(jsTransforms);
}

}
}
