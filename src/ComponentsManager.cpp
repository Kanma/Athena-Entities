/**	@file	ComponentsManager.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::ComponentsManager'
*/

#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Log/LogManager.h>

// #if ATHENA_ENTITIES_SCRIPTING
// #   include <Athena-Entities/IComponentsCreator.h>
// #   include <Athena-Scripting/ScriptingManager.h>
// #endif


using namespace Athena::Entities;
using namespace Athena::Log;
using namespace Athena::Utils;
using namespace std;

// #if ATHENA_ENTITIES_SCRIPTING
//     using namespace Athena::Scripting;
// #endif


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Components manager";


/********************************** STATIC ATTRIBUTES ***********************************/

/// The instance of the singleton
template<> ComponentsManager* Singleton<ComponentsManager>::ms_Singleton = 0;


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

ComponentsManager::ComponentsManager()
// #if ATHENA_ENTITIES_SCRIPTING
// : m_pCreator(0)
// #endif
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
// #if ATHENA_ENTITIES_SCRIPTING
//      if (m_types[strType].bPointer)
//      {
//          pComponent = m_types[strType].pMethod(strName, pList);
//      }
//      else if (m_pCreator)
//      {
//          pComponent = m_pCreator->createComponent(m_types[strType].strClass, strName, pList);
//      }
// #else
		pComponent = m_types[strType](strName, pList);
// #endif
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

// #if ATHENA_ENTITIES_SCRIPTING
// 
//  // Determines if the component is a C++ or a Python one
//  if (m_types[pComponent->getType()].bPointer)
//      delete pComponent;
//  else if (m_pCreator)
//      m_pCreator->destroyComponent(pComponent);
// 
// #else

    // Actual destruction
	delete pComponent;

// #endif
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

	// Add the new type to the list
// #if ATHENA_ENTITIES_SCRIPTING
// 
//  tCreationInfos infos;
//  infos.bPointer = true;
//  infos.pMethod = pCreationMethod;
//  m_types[strType] = infos;
// 
// #else

    m_types[strType] = pCreationMethod;

// #endif
}

//-----------------------------------------------------------------------

// #if ATHENA_ENTITIES_SCRIPTING
// 
// void ComponentsManager::registerType(const std::string& strType,
//                                   const std::string& strClass)
// {
//  assert(!strType.empty() && "The type's name is empty");
//  assert(!strClass.empty() && "Invalid Python class");
// 
//  // Declarations
//  std::map<String, tRepresentationPartCreationInfos>::iterator iter;
//  tRepresentationPartCreationInfos infos;
// 
//  ATHENA_LOG_EVENT("Registering a new type of component: '" + strType + "'");
// 
//  // Search if the type is already defined
//  iter = m_types.find(strType);
//  if (iter != m_types.end())
//  {
//      ATHENA_LOG_ERROR("The type of component '" + strType + "' was already registered");
//      return;
//  }
// 
//  // Add the new type to the list
//  infos.bPointer      = false;
//  infos.strClass      = strClass;
//  m_types[strType]    = infos;
// }
// 
// #endif
