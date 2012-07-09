#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<ComponentsList>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> ComponentsList_New(const Arguments& args)
{
    // New C++ list
    if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new ComponentsList());
    }

    // Wrapper around an existing C++ list
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        ComponentsList* pList = static_cast<ComponentsList*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pList, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nComponentsList()\nComponentsList(<C++ components list>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> ComponentsList_GetEntity(Local<String> property, const AccessorInfo &info)
{
    ComponentsList* ptr = GetPtr(info.This());
    assert(ptr);

    return toJavaScript(ptr->getEntity());
}

//-----------------------------------------------------------------------

Handle<Value> ComponentsList_GetScene(Local<String> property, const AccessorInfo &info)
{
    ComponentsList* ptr = GetPtr(info.This());
    assert(ptr);

    return toJavaScript(ptr->getScene());
}

//-----------------------------------------------------------------------

Handle<Value> ComponentsList_GetNbComponents(Local<String> property, const AccessorInfo &info)
{
    ComponentsList* ptr = GetPtr(info.This());
    assert(ptr);

    return Uint32::New(ptr->getNbComponents());
}


/**************************************** METHODS ***************************************/

Handle<Value> ComponentsList_RemoveAllComponents(const Arguments& args)
{
    ComponentsList* ptr = GetPtr(args.This());
    assert(ptr);

    ptr->removeAllComponents();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> ComponentsList_GetComponent(const Arguments& args)
{
    HandleScope handle_scope;

    ComponentsList* ptr = GetPtr(args.This());
    assert(ptr);

    Component* pComponent = 0;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        pComponent = ptr->getComponent(args[0]->ToUint32()->Value());
    }
    else if (((args.Length() == 1) || (args.Length() == 2)) && args[0]->IsObject())
    {
        tComponentID id = fromJSComponentID(args[0]);
        bool bInAllScene = false;

        if ((args.Length() == 2) && args[1]->IsBoolean())
            bInAllScene = args[1]->ToBoolean()->Value();

        pComponent = ptr->getComponent(id, bInAllScene);
    }

    if (!pComponent)
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetComponent(id, in_all_scene)\ngetComponent(index)"));

    return handle_scope.Close(toJavaScript(pComponent));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_ComponentsList(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> components_list = pManager->getClassTemplate("Athena.Entities.ComponentsList");

    if (components_list.IsEmpty())
    {
        // Declaration of the class
        components_list = FunctionTemplate::New(ComponentsList_New);
        components_list->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(components_list, "entity",       ComponentsList_GetEntity, 0);
        AddAttribute(components_list, "scene",        ComponentsList_GetScene, 0);
        AddAttribute(components_list, "nbComponents", ComponentsList_GetNbComponents, 0);

        // Methods
        AddMethod(components_list, "removeAllComponents", ComponentsList_RemoveAllComponents);
        AddMethod(components_list, "getComponent",        ComponentsList_GetComponent);

        pManager->declareClassTemplate("Athena.Entities.ComponentsList", components_list);
    }

    // Add the class to the parent
    return parent->Set(String::New("ComponentsList"), components_list->GetFunction());
}
