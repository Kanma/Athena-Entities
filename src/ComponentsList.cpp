/**	@file	ComponentsList.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::ComponentsList'
*/

#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scene.h>


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
    // If the components must not be destroyed, no special care is needed
    if (!bDestroy)
    {
    	while (!m_components.empty())
    		removeComponent(m_components.front(), bDestroy);
    
        return;
    }

    // We need to be carefull here: the transforms components must be removed
    // last, and the transforms component of the entity owning the list (if any)
    // must be the last one removed
    
    destroyAllComponentsOfType(COMP_OTHER);
    destroyAllComponentsOfType(COMP_DEBUG);
    destroyAllComponentsOfType(COMP_PHYSICAL);
    destroyAllComponentsOfType(COMP_AUDIO);
    destroyAllComponentsOfType(COMP_VISUAL);
    
    // Destroy the thransforms components (the first in the list is the one of
    // the entity)
	while (!m_components.empty())
		removeComponent(m_components.back(), bDestroy);
}

//-----------------------------------------------------------------------

void ComponentsList::destroyAllComponentsOfType(tComponentType type)
{
    // Assertions
    assert(ComponentsManager::getSingletonPtr());

    // Declarations
	tComponentsList::const_iterator iter, iterEnd;
    tComponentsList originalList = m_components;
    tComponentsList deleteList;
    
    // Split the list of components in two
    m_components.clear();
	for (iter = originalList.begin(), iterEnd = originalList.end(); iter != iterEnd; ++iter)
	{
		if ((*iter)->getID().type == type)
			deleteList.push_back(*iter);
	    else
			m_components.push_back(*iter);
	}
	
	// Delete the components of the given type
	for (iter = deleteList.begin(), iterEnd = deleteList.end(); iter != iterEnd; ++iter)
        ComponentsManager::getSingletonPtr()->destroy(*iter);
}

//-----------------------------------------------------------------------

Component* ComponentsList::getComponent(const tComponentID& id, bool bInAllScene) const
{
    // Test if the component ID correspond to this list (no entity name, or specifying the
    // name of the entity owning this list)
    if (id.strEntity.empty() || (m_pEntity && (id.strEntity == m_pEntity->getName())))
    {
    	tComponentsList::const_iterator iter, iterEnd;
    	for (iter = m_components.begin(), iterEnd = m_components.end(); iter != iterEnd; ++iter)
    	{
    		if ((*iter)->getID() == id)
    			return *iter;
    	}
    }
    
    // Otherwise search in the scene
    else if (bInAllScene)
    {
        Scene* pScene = m_pScene;
        if (!pScene && m_pEntity)
            pScene = m_pEntity->getScene();
        
        if (pScene)
        {
            Entity* pEntity = pScene->getEntity(id.strEntity);
            if (pEntity)
                return pEntity->getComponent(id);
        }
    }

    return 0;
}
