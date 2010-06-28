/**	@file	Scene.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Scene'
*/

#include <Athena-Entities/Scene.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Utils/PropertiesList.h>
#include <Athena-Core/Log/LogManager.h>


using namespace Athena::Entities;
using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace Athena::Log;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Scene";


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

Scene::Scene(const std::string& strName)
: m_strName(strName), m_bEnabled(true), m_bShown(false)
{
	// Assertions
	assert(ScenesManager::getSingletonPtr());

    m_components._setScene(this);
    memset(m_mainComponents, 0, 3 * sizeof(Component*));

	ScenesManager::getSingletonPtr()->_registerScene(this);
}

//-----------------------------------------------------------------------

Scene::~Scene()
{
	// Assertions
	assert(ScenesManager::getSingletonPtr());

	if (m_bEnabled)
		enable(false);

	destroyAll();

	ScenesManager::getSingletonPtr()->_destroyScene(this);
}


/****************************** MANAGEMENT OF THE SCENE *********************************/

void Scene::enable(bool bEnabled)
{
	if (bEnabled == m_bEnabled)
		return;

	m_bEnabled = bEnabled;

	if (m_bEnabled)
	{
		m_signals.fire(SIGNAL_SCENE_ENABLED, new Variant(getName()));
	}
	else
	{
		if (m_bShown)
			hide();

		m_signals.fire(SIGNAL_SCENE_DISABLED, new Variant(getName()));
	}
}

//-----------------------------------------------------------------------

void Scene::show()
{
	// Assertions
	assert(ScenesManager::getSingletonPtr() && "There isn't an Scene manager's instance");
	assert(m_bEnabled);

    ScenesManager* pScenesManager = ScenesManager::getSingletonPtr();

	// Notify the scenes manager
	pScenesManager->_onSceneShown(this);
	m_bShown = true;

	// Fire the 'shown' signal
	m_signals.fire(SIGNAL_SCENE_SHOWN);
}

//-----------------------------------------------------------------------

void Scene::hide()
{
	// Assertions
	assert(ScenesManager::getSingletonPtr() && "There isn't an Scene manager's instance");

    ScenesManager* pScenesManager = ScenesManager::getSingletonPtr();

	// Notify the scenes manager
	pScenesManager->_onSceneHidden(this);
	m_bShown = false;

	// Fire the 'hidden' signal
	m_signals.fire(SIGNAL_SCENE_HIDDEN);
}


/******************************* MANAGEMENT OF THE ENTITIES *****************************/

Entity* Scene::create(const std::string& strName, Entity* pParent)
{
	// Assertions
	assert(!strName.empty() && "Invalid entity name");
	assert(!getEntity(strName) && "Entity name already used");

	if (pParent && (pParent->getScene() != this))
		return 0;

	Entity* pEntity = new Entity(strName, this, pParent);

	m_entities.push_back(pEntity);

	return pEntity;
}

//-----------------------------------------------------------------------

Entity* Scene::getEntity(const std::string& strName)
{
	assert(!strName.empty() && "The name is empty");

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = m_entities.begin(), iterEnd = m_entities.end(); iter != iterEnd; ++iter)
	{
		if ((*iter)->getName() == strName)
		{
			// Return it
			return (*iter);
		}
	}

	// Not found
	return 0;
}

//-----------------------------------------------------------------------

void Scene::destroy(const std::string& strName)
{
	// Assertions
	assert(!strName.empty() && "The name is empty");

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = m_entities.begin(), iterEnd = m_entities.end(); iter != iterEnd; ++iter)
	{
		if ((*iter)->getName() == strName)
		{
			delete (*iter);
			m_entities.erase(iter);
			return;
		}
	}
}

//-----------------------------------------------------------------------

void Scene::destroy(Entity* pEntity)
{
	// Assertions
	assert(pEntity && "Invalid entity");

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = m_entities.begin(), iterEnd = m_entities.end(); iter != iterEnd; ++iter)
	{
		if (*iter == pEntity)
		{
			delete pEntity;
			m_entities.erase(iter);
			return;
		}
	}
}

//-----------------------------------------------------------------------

void Scene::destroyAll()
{
	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = m_entities.begin(), iterEnd = m_entities.end(); iter != iterEnd; ++iter)
		delete (*iter);

	m_entities.clear();
}

//-----------------------------------------------------------------------

void Scene::transfer(const std::string& strName, Scene* pSrcScene)
{
	// Assertions
	assert(!strName.empty() && "The name is empty");
	assert(pSrcScene);

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = pSrcScene->m_entities.begin(), iterEnd = pSrcScene->m_entities.end();
		 iter != iterEnd; ++iter)
	{
		if ((*iter)->getName() == strName)
		{
			Entity* pEntity = *iter;
			pSrcScene->m_entities.erase(iter);
			pEntity->m_pScene = this;
			m_entities.push_back(pEntity);
			return;
		}
	}
}

//-----------------------------------------------------------------------

void Scene::transfer(Entity* pEntity)
{
	// Assertions
	assert(pEntity && "Invalid entity");
	assert(pEntity->getScene());
	assert(pEntity->getScene() != this);

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Search the entity
	for (iter = pEntity->getScene()->m_entities.begin(), iterEnd = pEntity->getScene()->m_entities.end();
		 iter != iterEnd; ++iter)
	{
		if (*iter == pEntity)
		{
			pEntity->getScene()->m_entities.erase(iter);
			pEntity->m_pScene = this;
			m_entities.push_back(pEntity);
			return;
		}
	}
}
