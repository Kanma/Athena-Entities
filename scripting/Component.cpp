#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Scripting/Conversions.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Component>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Component_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new Component(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Component* pComponent = static_cast<Component*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nComponent(name, components_list)\nComponent(<C++ component>)"));
}


/************************************** PROPERTIES *************************************/

Handle<Value> Component_GetType(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(String::New(ptr->getType().c_str()));
}

//-----------------------------------------------------------------------

Handle<Value> Component_GetID(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getID()));
}

//-----------------------------------------------------------------------

Handle<Value> Component_GetName(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(String::New(ptr->getName().c_str()));
}

//-----------------------------------------------------------------------

Handle<Value> Component_GetList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getList()));
}

//-----------------------------------------------------------------------

Handle<Value> Component_GetTransforms(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    if (ptr->getTransforms())
        return handle_scope.Close(toJavaScript(ptr->getTransforms()));

    return Handle<Value>();
}

//-----------------------------------------------------------------------

void Component_SetTransforms(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setTransforms(fromJSTransforms(value));
}

//-----------------------------------------------------------------------

Handle<Value> Component_GetSignalsList(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getSignalsList()));
}


/**************************************** METHODS ***************************************/

Handle<Value> Component_RemoveTransforms(const Arguments& args)
{
    HandleScope handle_scope;

    Component* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->removeTransforms();

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Component(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Entities.Component");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Utils.Describable").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(Component_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Utils.Describable"));

        // Attributes
        AddAttribute(component, "type",       Component_GetType, 0);
        AddAttribute(component, "id",         Component_GetID, 0);
        AddAttribute(component, "name",       Component_GetName, 0);
        AddAttribute(component, "list",       Component_GetList, 0);
        AddAttribute(component, "transforms", Component_GetTransforms, Component_SetTransforms);
        AddAttribute(component, "signals",    Component_GetSignalsList, 0);

        // Methods
        AddMethod(component, "removeTransforms", Component_RemoveTransforms);

        pManager->declareClassTemplate("Athena.Entities.Component", component);

        parent->Set(String::New("Component_TYPE"), String::New(Athena::Entities::Component::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("Component"), component->GetFunction());
}
