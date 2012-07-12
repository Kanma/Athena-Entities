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


#define FROM_JS(CLASS_NAME)                                                                \
CLASS_NAME* fromJS##CLASS_NAME(Handle<Value> value)                                        \
{                                                                                          \
    if (value->IsObject())                                                                 \
    {                                                                                      \
        CLASS_NAME* pObject = 0;                                                           \
        GetObjectPtr(value, &pObject);                                                     \
        return pObject;                                                                    \
    }                                                                                      \
                                                                                           \
    return 0;                                                                              \
}


#define CREATE_JS(CLASS_NAME)                                                              \
Handle<Object> createJS##CLASS_NAME()                                                      \
{                                                                                          \
    HandleScope handle_scope;                                                              \
                                                                                           \
    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate( \
                                                        "Athena.Entities." #CLASS_NAME);   \
                                                                                           \
    Handle<Object> jsObject = func->GetFunction()->NewInstance();                          \
                                                                                           \
    return handle_scope.Close(jsObject);                                                   \
}


#define TO_JS(CLASS_NAME)                                                                  \
Handle<Value> toJavaScript(CLASS_NAME* pObject)                                            \
{                                                                                          \
    HandleScope handle_scope;                                                              \
                                                                                           \
    if (!pObject)                                                                          \
        return Handle<Value>();                                                            \
                                                                                           \
    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate( \
                                                        "Athena.Entities." #CLASS_NAME);   \
                                                                                           \
    Handle<Value> argv[1];                                                                 \
    argv[0] = External::Wrap(pObject);                                                     \
                                                                                           \
    Handle<Object> jsObject = func->GetFunction()->NewInstance(1, argv);                   \
                                                                                           \
    return handle_scope.Close(jsObject);                                                   \
}


#define IMPLEMENT_CONVERSIONS(CLASS_NAME)                                                  \
    FROM_JS(CLASS_NAME);                                                                   \
    CREATE_JS(CLASS_NAME);                                                                 \
    TO_JS(CLASS_NAME);



namespace Athena {
namespace Entities {


IMPLEMENT_CONVERSIONS(Animation)
IMPLEMENT_CONVERSIONS(AnimationsMixer)
IMPLEMENT_CONVERSIONS(ComponentAnimation)
IMPLEMENT_CONVERSIONS(ComponentsList)
IMPLEMENT_CONVERSIONS(Component)
IMPLEMENT_CONVERSIONS(Entity)
IMPLEMENT_CONVERSIONS(Scene)
IMPLEMENT_CONVERSIONS(Transforms)

//-----------------------------------------------------------------------

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
    HandleScope handle_scope;

    Local<Object> object = ScriptingManager::getSingletonPtr()->execute("new Athena.Entities.ComponentID('" + value.toString() + "');")->ToObject();

    return handle_scope.Close(object);
}

}
}
