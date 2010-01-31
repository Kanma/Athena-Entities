/**	@file	Entity.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Entity'
*/

#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scene.h>
//#include "Athena_Entities_Animation_CAnimationsMixer.h"
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Log/LogManager.h>


using namespace Athena::Entities;
using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace Athena::Log;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char*	__CONTEXT__	= "Entity";


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

Entity::Entity(const std::string& strName, Scene* pScene, Entity* pParent)
: m_strName(strName), m_pScene(pScene), m_pParent(0), m_bEnabled(true),
//  m_pAnimationsMixer(0),
  m_pTransforms(0)
{
	// Assertions
	assert(!strName.empty() && "Invalid name");
	assert(pScene);

	// Initializations
	m_components._setEntity(this);

	m_pTransforms = new Transforms("Transforms", &m_components);

	// If a parent was specified, ask it to add this entity in its children's list
	if (pParent)
		pParent->addChild(this);
}

//-----------------------------------------------------------------------

Entity::~Entity()
{
    // if (m_pAnimationsMixer)
    // {
    //  delete m_pAnimationsMixer;
    //  m_pAnimationsMixer = 0;
    // }

	// Destroy all the children of this entity
	destroyAllChildren();

	// Notify the parent that this entity is destroyed
	if (m_pParent)
	{
		m_pParent->removeChild(this);
		m_pParent = 0;
	}

	m_components.removeAllComponents();
}


/****************************** MANAGEMENT OF THE ENTITY *******************************/

void Entity::enable(bool bEnabled)
{
	if (bEnabled == m_bEnabled)
		return;

	m_bEnabled = bEnabled;

	if (m_bEnabled)
		m_signals.fire(SIGNAL_ENTITY_ENABLED, new Variant(getName()));
	else
		m_signals.fire(SIGNAL_ENTITY_DISABLED, new Variant(getName()));
}


/********************* MANAGEMENT OF THE PARENT/CHILDREN RELATION **********************/

void Entity::addChild(Entity* pChild)
{
	// Assertions
	assert(pChild && "Invalid child");

	// If the child has already a parent, detach it first
	if (pChild->m_pParent)
		pChild->m_pParent->removeChild(pChild);
		
	// Add it to the list of this entity
	m_children.push_back(pChild);
	pChild->m_pParent = this;

	pChild->getTransforms()->setTransformsOrigin(m_pTransforms);
}

//-----------------------------------------------------------------------

void Entity::removeChild(Entity* pChild)
{
	// Assertions
	assert(pChild && "Invalid child");

	// Declarations
	tEntitiesNativeIterator iter, iterEnd;

	// Remove the child from the list of this entity
	for (iter = m_children.begin(), iterEnd = m_children.end(); iter != iterEnd; ++iter)
	{
		if (*iter == pChild)
		{
			(*iter)->m_pParent = 0;
			(*iter)->getTransforms()->setTransformsOrigin(0);
			m_children.erase(iter);
			return;
		}
	}
}

//-----------------------------------------------------------------------

void Entity::destroyAllChildren()
{
	// Destroy the children
	while (!m_children.empty())
		m_pScene->destroy(m_children.front());
}


/***************************** MANAGEMENT OF THE ANIMATIONS ****************************/

// CAnimationsMixer* CEntity::createAnimationsMixer()
// {
//  if (!m_pAnimationsMixer)
//      m_pAnimationsMixer = new CAnimationsMixer();
// 
//  return m_pAnimationsMixer;
// }
// 
