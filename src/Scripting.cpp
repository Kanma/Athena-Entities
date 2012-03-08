/** @file   Scripting.cpp
    @author Philip Abbet

    Implementation of the scripting-related functions of the Athena-Entities module
*/

#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/ScriptingManager.h>
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

}
}
