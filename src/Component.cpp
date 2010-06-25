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
: m_id(COMP_OTHER, strName), m_pList(pList), m_pTransforms(0)  
{
	// Assertions
	assert(!strName.empty() && "Invalid name");
	assert(pList && "Invalid list");

    if (pList->getEntity())
    {
        m_id.strEntity = pList->getEntity()->getName();

        if (pList->getEntity()->getTransforms())
            m_pTransforms = pList->getEntity()->getTransforms();
        else if (pList->getEntity()->getParent())
            m_pTransforms = pList->getEntity()->getParent()->getTransforms();
        
        if (m_pTransforms)
            _connectToParentTransformsSignals();
    }

	pList->_addComponent(this);
}

//-----------------------------------------------------------------------

Component::~Component()
{
	// Assertions
	assert(m_pList);

    // // Remove the reference to the transforms origin (if any)
    if (m_pTransforms)
    {
        _disconnectFromParentTransformsSignals();
        m_pTransforms = 0;
    }

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


/******************* MANAGEMENT OF THE ORIGIN OF THE TRANSFORMATIONS *******************/

void Component::setTransforms(Transforms* pTransforms)
{
    // Assertions
    assert(m_pList);

    // Unregister to the signals of the current transforms
    if (m_pTransforms)
        _disconnectFromParentTransformsSignals();

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
        SignalsList* pSignals = pNewTransforms->getSignalsList();
        pSignals->connect(SIGNAL_COMPONENT_DESTROYED, this, &Component::onParentTransformsDestroyed);
    }

    // Fire a signal about the change of origin
    if (pNewTransforms)
        m_signals.fire(SIGNAL_COMPONENT_PARENT_TRANSFORMS_CHANGED, new Variant(pNewTransforms->getID().toString()));
    else
        m_signals.fire(SIGNAL_COMPONENT_PARENT_TRANSFORMS_CHANGED, new Variant(tComponentID(COMP_NONE).toString()));

    // Stores a reference to the new origin
    m_pTransforms = pNewTransforms;

    // Fire a 'transforms changed' signal
    if (pNewTransforms)
        pNewTransforms->getSignalsList()->fire(SIGNAL_COMPONENT_TRANSFORMS_CHANGED);
}

//-----------------------------------------------------------------------

void Component::setTransforms(const tComponentID& id)
{
    // Assertions
    assert(m_pList);

    setTransforms(Transforms::cast(m_pList->getComponent(id)));
}

//-----------------------------------------------------------------------

void Component::_connectToParentTransformsSignals()
{
    if (m_pTransforms)
    {
        SignalsList* pSignals = m_pTransforms->getSignalsList();
        pSignals->connect(SIGNAL_COMPONENT_DESTROYED, this, &Component::onParentTransformsDestroyed);
    }
}

//-----------------------------------------------------------------------

void Component::_disconnectFromParentTransformsSignals()
{
    if (m_pTransforms)
    {
        SignalsList* pSignals = m_pTransforms->getSignalsList();
        pSignals->disconnect(SIGNAL_COMPONENT_DESTROYED, this, &Component::onParentTransformsDestroyed);
    }
}


/**************************************** SLOTS ****************************************/

void Component::onParentTransformsDestroyed(Utils::Variant* pValue)
{
    // Assertions
    assert(m_pTransforms);

    setTransforms(0);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* Component::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Describable::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Parent Transforms
    if (m_pTransforms)
        pProperties->set("transforms", new Variant(m_pTransforms->getID().toString()));

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

	// Parent Transforms
    if (strName == "transforms")
    {
        tComponentID id(pValue->toString());

        if (id.type != COMP_NONE)
        {
            Transforms* pParent = Transforms::cast(m_pList->getComponent(id));
            setTransforms(pParent);

            if (!pParent)
                bUsed = false;
        }
        else
        {
            setTransforms(0);
        }
    }

	// Destroy the value
	delete pValue;

	return bUsed;
}
