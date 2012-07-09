#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<ComponentsManager>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> ComponentsManager_New(const Arguments& args)
{
    // Return a wrapper around the existing singleton object
    return SetObjectPtr(args.This(), ComponentsManager::getSingletonPtr(), &NoOpWeakCallback);
}


/**************************************** METHODS ***************************************/

Handle<Value> ComponentsManager_Create(const Arguments& args)
{
    HandleScope handle_scope;

    ComponentsManager* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 3) && args[0]->IsString() && args[1]->IsString() && args[2]->IsObject())
    {
        return handle_scope.Close(toJavaScript(ptr->create(*String::AsciiValue(args[0]->ToString()),
                                                           *String::AsciiValue(args[1]->ToString()),
                                                           fromJSComponentsList(args[2]))));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreate(type, name, components_list)"));
}

//-----------------------------------------------------------------------

Handle<Value> ComponentsManager_Destroy(const Arguments& args)
{
    HandleScope handle_scope;

    ComponentsManager* ptr = GetPtr(args.This());
    assert(ptr);

    if ((args.Length() == 1) && args[0]->IsObject())
        ptr->destroy(fromJSComponent(args[0]));
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndestroy(component)"));

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_ComponentsManager(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> components_manager = pManager->getClassTemplate("Athena.Entities.ComponentsManager");

    if (components_manager.IsEmpty())
    {
        // Declaration of the class
        components_manager = FunctionTemplate::New(ComponentsManager_New);
        components_manager->InstanceTemplate()->SetInternalFieldCount(1);

        // Methods
        AddMethod(components_manager, "create",  ComponentsManager_Create);
        AddMethod(components_manager, "destroy", ComponentsManager_Destroy);

        pManager->declareClassTemplate("Athena.Entities.ComponentsManager", components_manager);
    }

    // Add the class to the parent
    return parent->Set(String::New("ComponentsManager"), components_manager->GetFunction());
}
