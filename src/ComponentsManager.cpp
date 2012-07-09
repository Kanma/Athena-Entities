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
    registerType(Component::TYPE, (ComponentCreationMethod*) &Component::create);

    // Register the types of uncategorized components known by the engine
    registerType(Transforms::TYPE, (ComponentCreationMethod*) &Transforms::create);
}

//-----------------------------------------------------------------------

ComponentsManager::~ComponentsManager()
{
    ATHENA_LOG_EVENT("Destruction");
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

    // Search the creation method of the type
    if (m_types.find(strType) != m_types.end())
    {
        // Use it to create the component
        pComponent = m_types[strType](strName, pList);
    }
    else
    {
        ATHENA_LOG_ERROR("Unknow component's type: " + strType);
    }

    if (!pComponent)
        ATHENA_LOG_ERROR("Failed to create a component of type '" + strType + "' with the name '" + strName + "'");

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


/************************* REGISTRATION OF NEW TYPES OF COMPONENTS **********************/

void ComponentsManager::registerType(const std::string& strType,
                                     ComponentCreationMethod* pCreationMethod)
{
    // Assertions
    assert(!strType.empty() && "The type's name is empty");
    assert(pCreationMethod && "Invalid creation method");

    // Declarations
    tCreationsInfosNativeIterator       iter;

    ATHENA_LOG_EVENT("Registering a new type of component: '" + strType + "'");

    // Search if the type is already defined
    iter = m_types.find(strType);
    if (iter != m_types.end())
    {
        ATHENA_LOG_ERROR("The type of component '" + strType + "' was already registered");
        return;
    }

    m_types[strType] = pCreationMethod;
}
