/**	@file	Component.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Component'
*/

#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/Variant.h>


using namespace Athena;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Signals;
using namespace std;


/************************************** CONSTANTS ***************************************/

// Type of component
const std::string Component::TYPE = "Athena/Component";


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

Component::Component(const std::string& strName, ComponentsList* pList)
: m_id(COMP_OTHER, strName), m_pList(pList), m_pTransformsOrigin(0)  
{
	// Assertions
	assert(!strName.empty() && "Invalid name");
	assert(pList && "Invalid list");

    if (pList->getEntity())
        m_id.strEntity = pList->getEntity()->getName();

	pList->_addComponent(this);
}

//-----------------------------------------------------------------------

Component::~Component()
{
	// Assertions
	assert(m_pList);

    // // Remove the reference to the transforms origin (if any)
    if (m_pTransformsOrigin)
        setTransformsOrigin(0);

	// Fire a 'component destroyed' signal
	m_signals.fire(SIGNAL_COMPONENT_DESTROYED);

	// Remove the component from the list
	m_pList->removeComponent(this, false);
}

//-----------------------------------------------------------------------

Component* Component::create(const std::string& strName, ComponentsList* pList)
{
	return new Component(strName, pList);
}


/*************************************** METHODS ***************************************/

Transforms* Component::getTransforms() const
{
    // Assertions
    assert(m_pList);

    // If we have a transforms origin, return it
    if (m_pTransformsOrigin)
        return m_pTransformsOrigin;

    // If we are part to an entity
    Entity* pEntity = m_pList->getEntity();
    if (pEntity)
    {
        // If we aren't its transforms component, return it
        if (pEntity->getTransforms() != this)
            return pEntity->getTransforms();
        
        // If our entity has a parent, return its transforms
        if (pEntity->getParent())
            return pEntity->getParent()->getTransforms();
    }

    // No transform component affect this one
    return 0;
}


/******************* MANAGEMENT OF THE ORIGIN OF THE TRANSFORMATIONS *******************/

void Component::setTransformsOrigin(Transforms* pTransforms)
{
    // Assertions
    assert(m_pList);

    // Unregister to the signals of the current transforms
    Transforms* pCurrentTransforms = getTransforms();
    if (pCurrentTransforms)
    {
        SignalsList* pSignals = pCurrentTransforms->getSignalsList();
        pSignals->disconnect(SIGNAL_COMPONENT_DESTROYED, this, &Component::onTransformsOriginDestroyed);
    }

    // Determine the new transforms
    Transforms* pNewTransforms = pTransforms;
    if (!pNewTransforms)
    {
        Entity* pEntity = m_pList->getEntity();
        if (pEntity)
        {
            if (pEntity->getTransforms() != this)
                pNewTransforms = pEntity->getTransforms();
            else if (pEntity->getParent())
                pNewTransforms = pEntity->getParent()->getTransforms();
        }
    }
            
    // Register to the signals of the new transforms
    if (pNewTransforms)
    {
        SignalsList* pSignals = pTransforms->getSignalsList();
        pSignals->connect(SIGNAL_COMPONENT_DESTROYED, this, &Component::onTransformsOriginDestroyed);
    }

    // Fire a signal about the change of origin
    if (pNewTransforms)
        m_signals.fire(SIGNAL_COMPONENT_PARENT_TRANSFORMS_CHANGED, new Variant(pNewTransforms->getID().toString()));
    else
        m_signals.fire(SIGNAL_COMPONENT_PARENT_TRANSFORMS_CHANGED, new Variant(tComponentID(COMP_NONE).toString()));

    // Stores a reference to the new origin
    m_pTransformsOrigin = pTransforms;

    // Fire a 'transforms changed' signal
    if (pNewTransforms)
        pNewTransforms->getSignalsList()->fire(SIGNAL_COMPONENT_TRANSFORMS_CHANGED);
}

//-----------------------------------------------------------------------

void Component::setTransformsOrigin(const tComponentID& id)
{
    // Assertions
    assert(m_pList);

    setTransformsOrigin(Transforms::cast(m_pList->getComponent(id)));
}


/**************************************** SLOTS ****************************************/

void Component::onTransformsOriginDestroyed(Utils::Variant* pValue)
{
    // Assertions
    assert(m_pTransformsOrigin);

    setTransformsOrigin(0);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* Component::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Describable::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Transforms origin
    if (m_pTransformsOrigin)
        pProperties->set("transforms-origin", new Variant(m_pTransformsOrigin->getID().toString()));
    else
		pProperties->set("transforms-origin", new Variant(tComponentID(COMP_NONE).toString()));

	// Returns the list
	return pProperties;
}


//-----------------------------------------------------------------------

bool Component::setProperty(const std::string& strCategory, const std::string& strName,
							Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return Component::setProperty(strName, pValue);

	return Describable::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool Component::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Declarations
	bool bUsed = true;

	// Transforms origin
    if (strName == "transforms-origin")
    {
        tComponentID id(pValue->toString());

        if (id.type != COMP_NONE)
        {
            Transforms* pOrigin = Transforms::cast(m_pList->getComponent(id));
            if (pOrigin)
                setTransformsOrigin(pOrigin);
            else
                bUsed = false;
        }
        else
        {
            setTransformsOrigin(0);
        }
    }

	// Destroy the value
	delete pValue;

	return bUsed;
}
