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

void ComponentsList::_removeComponent(Component* pComponent)
{
    // Assertions
	assert(pComponent);
    assert(ComponentsManager::getSingletonPtr());

    // Unlink the component
    pComponent->removeTransforms();
    pComponent->unlink();

    // Remove it from the list
    Component::tComponentsList::iterator iter, iterEnd;
	for (iter = m_components.begin(), iterEnd = m_components.end(); iter != iterEnd; ++iter)
	{
		if (*iter == pComponent)
		{
			m_components.erase(iter);
			break;
		}
	}
}

//-----------------------------------------------------------------------

void ComponentsList::removeAllComponents()
{
    // Unlink all the components
    Component::tComponentsIterator iter(m_components.begin(), m_components.end());
    while (iter.hasMoreElements())
    {
        Component* pComponent = iter.getNext();
        pComponent->removeTransforms();
        pComponent->unlink();
    }
    
    // Destroy the components
    while (!m_components.empty())
        ComponentsManager::getSingletonPtr()->destroy(m_components.front());
}

//-----------------------------------------------------------------------

Component* ComponentsList::getComponent(const tComponentID& id, bool bInAllScene) const
{
    // Test if the component ID correspond to this list (no entity name, or specifying the
    // name of the entity owning this list)
    if (id.strEntity.empty() || (m_pEntity && (id.strEntity == m_pEntity->getName())))
    {
    	Component::tComponentsList::const_iterator iter, iterEnd;
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
