/** @file   ComponentsManager.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Entities::ComponentsManager'
*/

#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Log/LogManager.h>

#if ATHENA_ENTITIES_SCRIPTING
    #include <Athena-Entities/Scripting.h>
#endif


using namespace Athena::Entities;
using namespace Athena::Log;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Components manager";


/********************************** STATIC ATTRIBUTES ***********************************/

/// The instance of the singleton
template<> ComponentsManager* Singleton<ComponentsManager>::ms_Singleton = 0;


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

ComponentsManager::ComponentsManager()
{
    ATHENA_LOG_EVENT("Creation");

    // Register the base type of all the components
    registerType<Component>();

    // Register the types of uncategorized components known by the engine
    registerType<Transforms>();
}

//-----------------------------------------------------------------------

ComponentsManager::~ComponentsManager()
{
    ATHENA_LOG_EVENT("Destruction");

    tCreationsInfosIterator iter(m_types.begin(), m_types.end());
    while (iter.hasMoreElements())
        delete iter.getNext();
}

//-----------------------------------------------------------------------

ComponentsManager& ComponentsManager::getSingleton()
{
    assert(ms_Singleton);
    return *ms_Singleton;
}

//-----------------------------------------------------------------------

ComponentsManager* ComponentsManager::getSingletonPtr()
{
    return ms_Singleton;
}


/****************************** MANAGEMENT OF THE COMPONENTS ****************************/

Component* ComponentsManager::create(const std::string& strType, const std::string& strName,
                                     ComponentsList* pList)
{
    // Assertions
    assert(!strType.empty() && "Invalid type name");
    assert(!strName.empty() && "Invalid component name");
    assert(pList && "Invalid list");

    // Declarations
    Component* pComponent = 0;

    ATHENA_LOG_DEBUG("Creating a new component of type '" + strType + "' with the name '" + strName + "'");

    // Search the creation infos of the type
    if (m_types.find(strType) != m_types.end())
    {
        // Use them to create the component
        pComponent = m_types[strType]->create(strName, pList);
        if (!pComponent)
            ATHENA_LOG_ERROR("Failed to create a component of type '" + strType + "' with the name '" + strName + "'");
    }
    else
    {
        ATHENA_LOG_ERROR("Failed to create the component '" + strName + "': unknown component type (" + strType + ")");
    }

    return pComponent;
}

//-----------------------------------------------------------------------

void ComponentsManager::destroy(Component* pComponent)
{
    // Assertions
    assert(pComponent && "Invalid component");
    assert(m_types.find(pComponent->getType()) != m_types.end());

    // Fire a 'component destroyed' signal
    pComponent->getSignalsList()->fire(SIGNAL_COMPONENT_DESTROYED);

    // Remove the component form its list
    pComponent->getList()->_removeComponent(pComponent);

    // Actual destruction
    delete pComponent;
}

//-----------------------------------------------------------------------

#if ATHENA_ENTITIES_SCRIPTING

v8::Handle<v8::Value> ComponentsManager::convertToJavaScript(Component* pComponent)
{
    // Assertions
    assert(pComponent);

    // Search the creation infos of the type
    if (m_types.find(pComponent->getType()) != m_types.end())
    {
        // Use them to wrap the component
        return m_types[pComponent->getType()]->convertToJavaScript(pComponent);
    }

    // Type not found, return a basic component
    return Entities::toJavaScript(pComponent);
}

#endif
