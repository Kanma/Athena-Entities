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

	if (m_pList->getEntity())
		m_id.strEntity = m_pList->getEntity()->getName();

	pList->_addComponent(this);
}

//-----------------------------------------------------------------------

Component::~Component()
{
	// Assertions
	assert(m_pList);

    m_pTransforms = 0;

    // Remove the link with all the refered components
    tComponentsIterator iter(m_referees.begin(), m_referees.end());
    while (iter.hasMoreElements())
        iter.getNext()->removeReferer(this);

    m_referees.clear();

    // Tell all the components that references us about the destruction
    iter = tComponentsIterator(m_referers.begin(), m_referers.end());
    while (iter.hasMoreElements())
        iter.getNext()->onComponentDestroyed(this);
    
    m_referers.clear();

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


/*************************** MANAGEMENT OF THE TRANSFORMATIONS **************************/

void Component::setTransforms(Transforms* pTransforms)
{
    // Assertions
    assert(m_pList);

    // Unregister to the signals of the current transforms
    if (m_pTransforms)
    {
        removeReferee(m_pTransforms);
        m_pTransforms->removeReferer(this);
        m_pTransforms = 0;
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
    
    // Stores a reference to the new transforms
    m_pTransforms = pNewTransforms;

    if (m_pTransforms)
	{
        addReferee(m_pTransforms);
        m_pTransforms->addReferer(this);

		// Do whatever we must do when our transforms change
		onTransformsChanged();
	}
}

//-----------------------------------------------------------------------

void Component::setTransforms(const tComponentID& id)
{
    // Assertions
    assert(m_pList);

    setTransforms(Transforms::cast(m_pList->getComponent(id)));
}

//-----------------------------------------------------------------------

void Component::removeTransforms()
{
	if (!m_pTransforms)
		return;

    removeReferee(m_pTransforms);
    m_pTransforms->removeReferer(this);

    m_pTransforms = 0;

	onTransformsChanged();
}
 
//-----------------------------------------------------------------------

void Component::onTransformsChanged()
{
}


/**************************** REFERERS / REFEREES MANAGEMENT ***************************/

void Component::onComponentDestroyed(Component* pReferee)
{
    // Assertions
    assert(pReferee);

    if (m_pTransforms == pReferee)
    {
        m_pTransforms = 0;
        setTransforms(0);
    }

    removeReferee(pReferee);
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
