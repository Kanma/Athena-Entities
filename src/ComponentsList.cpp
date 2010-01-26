/**	@file	ComponentsList.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::ComponentsList'
*/

#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Entity.h>


using namespace Athena;
using namespace Athena::Entities;
using namespace std;


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

ComponentsList::ComponentsList()
: m_pEntity(0), m_pScene(0)
{
}

//-----------------------------------------------------------------------

ComponentsList::~ComponentsList()
{
	removeAllComponents();
}


/************************************** METHODS *****************************************/

bool ComponentsList::_addComponent(Component* pComponent)
{
	assert(pComponent);

	if (getComponent(pComponent->getID()) != 0)
		return false;

	m_components.push_back(pComponent);

	return true;
}

//-----------------------------------------------------------------------

void ComponentsList::removeComponent(Component* pComponent, bool bDestroy)
{
    // Assertions
	assert(pComponent);
    assert(ComponentsManager::getSingletonPtr());

	tComponentsList::iterator iter, iterEnd;

	for (iter = m_components.begin(), iterEnd = m_components.end(); iter != iterEnd; ++iter)
	{
		if (*iter == pComponent)
		{
			m_components.erase(iter);

            if (bDestroy)
                ComponentsManager::getSingletonPtr()->destroy(pComponent);

			break;
		}
	}
}

//-----------------------------------------------------------------------

void ComponentsList::removeComponent(const tComponentID& id, bool bDestroy)
{
	Component* pComponent = getComponent(id);
	if (pComponent)
		removeComponent(pComponent, bDestroy);
}

//-----------------------------------------------------------------------

void ComponentsList::removeAllComponents(bool bDestroy)
{
	while (!m_components.empty())
		removeComponent(m_components.front(), bDestroy);
}

//-----------------------------------------------------------------------

Component* ComponentsList::getComponent(const tComponentID& id) const
{
	// Check that if an entity owns this list, the component ID specify the entity name
	if (id.strEntity.empty() && m_pEntity)
		return 0;

	// Test if the component belongs to the entity owning this list
    if (m_pEntity && (id.strEntity != m_pEntity->getName()))
    {
        // Ask the parent entity for the component (if the component is a transforms one)
        if ((id.type == COMP_TRANSFORMS) && m_pEntity->getParent())
            return m_pEntity->getParent()->getComponent(id);
        else
            return 0;
    }

	// Search the component
	tComponentsList::const_iterator iter, iterEnd;
	for (iter = m_components.begin(), iterEnd = m_components.end(); iter != iterEnd; ++iter)
	{
		if ((*iter)->getID() == id)
			return *iter;
	}

	return 0;
}
