/**	@file	Component.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Component'
*/

#include <Athena-Entities/Component.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Transforms.h>
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
    // This was done by our managers
    assert(m_linked_by.empty());
    assert(m_linked_to.empty());
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

    // Unlink from the current transforms
    if (m_pTransforms)
    {
        removeLinkTo(m_pTransforms);
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
        addLinkTo(m_pTransforms);

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

    removeLinkTo(m_pTransforms);
    m_pTransforms = 0;

	onTransformsChanged();
}

//-----------------------------------------------------------------------

void Component::onTransformsChanged()
{
}


/*********************************** LINKS MANAGEMENT **********************************/

void Component::mustUnlinkComponent(Component* pComponent)
{
    // Assertions
    assert(pComponent);

    if (m_pTransforms == pComponent)
        removeTransforms();

    removeLinkTo(pComponent);
}

//-----------------------------------------------------------------------

void Component::unlink()
{
    // Tell all the components linked to this one to unlink it
    while (!m_linked_by.empty())
        m_linked_by.front()->mustUnlinkComponent(this);

    // Unlink all the components we are linked with
    while (!m_linked_to.empty())
        mustUnlinkComponent(m_linked_to.front());
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
