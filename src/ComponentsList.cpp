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
	while (!m_components.empty())
		removeComponent(m_components.front(), bDestroy);
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
