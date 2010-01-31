/**	@file	Transforms.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Transforms'
*/

#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/PropertiesList.h>

using namespace Athena::Entities;
using namespace Athena::Math;
using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS ***************************************/

///< Name of the type of component
const std::string Transforms::TYPE = "Athena/Transforms";


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

Transforms::Transforms(const std::string& strName, ComponentsList* pList)
: Component(strName, pList), m_position(Vector3::ZERO), m_orientation(Quaternion::IDENTITY),
  m_scale(Vector3::UNIT_SCALE), m_fullPosition(Vector3::ZERO), m_fullOrientation(Quaternion::IDENTITY),
  m_fullScale(Vector3::UNIT_SCALE), m_bDirty(true), m_uiNbFullCalculationRequests(0),
  m_bInheritOrientation(true), m_bInheritScale(true)
{
	m_id.type = COMP_TRANSFORMS;

	// Signals handling
	m_signals.connect(SIGNAL_COMPONENT_TRANSFORMS_ORIGIN_CHANGED, this, &Transforms::onTransformsOriginChanged);

	// Use the transforms of the entity as our origin by default (if any)
	if (m_pList->getEntity() && m_pList->getEntity()->getTransforms())
		setTransformsOrigin(m_pList->getEntity()->getTransforms());
}

//-----------------------------------------------------------------------

Transforms::~Transforms()
{
	setTransformsOrigin(0);
}

//-----------------------------------------------------------------------

Transforms* Transforms::create(const std::string& strName, ComponentsList* pList)
{
	return new Transforms(strName, pList);
}

//-----------------------------------------------------------------------

Transforms* Transforms::cast(Component* pComponent)
{
	return dynamic_cast<Transforms*>(pComponent);
}


/*************************************** POSITION ***************************************/

void Transforms::setPosition(const Vector3& pos)
{
	m_position = pos;
	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::setPosition(Real x, Real y, Real z)
{
	setPosition(Vector3(x, y, z));
}

//-----------------------------------------------------------------------

void Transforms::translate(const Vector3& d, tTransformSpace relativeTo)
{
	Vector3 adjusted;

	switch(relativeTo)
	{
	case TS_LOCAL:
		// Position is relative to parent so transform downwards
		m_position += m_orientation * d;
		break;

	case TS_PARENT:
		m_position += d;
		break;

	case TS_WORLD:
		// Position is relative to parent so transform upwards
		if (m_pTransformsOrigin)
			m_position += (m_pTransformsOrigin->getWorldOrientation().Inverse() * d) / m_pTransformsOrigin->getWorldScale();
		else
			m_position += d;
		break;
	}

	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::translate(Real x, Real y, Real z, tTransformSpace relativeTo)
{
	translate(Vector3(x, y, z));
}

//-----------------------------------------------------------------------

Vector3 Transforms::getWorldPosition()
{
	if (m_bDirty)
		update();

	return m_fullPosition;
}


/*************************************** ORIENTATION ***************************************/

//-----------------------------------------------------------------------
void Transforms::setDirection(const Vector3& vec, tTransformSpace relativeTo,
							  const Vector3& localDirectionVector)
{
	// Do nothing if given a zero vector
	if (vec == Vector3::ZERO)
		return;

	// The direction we want the local direction point to
	Vector3 targetDir = vec.normalisedCopy();

	// Transform target direction to world space
	switch (relativeTo)
	{
	case TS_LOCAL:
		targetDir = getWorldOrientation() * targetDir;
		break;

	case TS_PARENT:
		if (m_bInheritOrientation && m_pTransformsOrigin)
			targetDir = m_pTransformsOrigin->getWorldOrientation() * targetDir;
		break;

	case TS_WORLD:
		// default orientation
		break;
	}

	// Calculate target orientation relative to world space
	Quaternion targetOrientation;

	// Get current local direction relative to world space
	const Quaternion& currentOrient = getWorldOrientation();
	Vector3 currentDir = currentOrient * localDirectionVector;

	if ((currentDir + targetDir).squaredLength() < 0.00005f)
	{
		// Oops, a 180 degree turn (infinite possible rotation axes)
		// Default to yaw i.e. use current UP
		targetOrientation =
			Quaternion(-currentOrient.y, -currentOrient.z, currentOrient.w, currentOrient.x);
	}
	else
	{
		// Derive shortest arc to new direction
		Quaternion rotQuat = currentDir.getRotationTo(targetDir);
		targetOrientation = rotQuat * currentOrient;
	}

	// Set target orientation, transformed to parent space
	if (m_pTransformsOrigin && m_bInheritOrientation)
		setOrientation(m_pTransformsOrigin->getWorldOrientation().UnitInverse() * targetOrientation);
	else
		setOrientation(targetOrientation);
}

//-----------------------------------------------------------------------

void Transforms::lookAt(const Vector3& targetPoint, tTransformSpace relativeTo,
						const Vector3& localDirectionVector)
{
	// Calculate our self origin relative to the given transform space
	Vector3 origin;
	switch (relativeTo)
	{
	case TS_WORLD:
		origin = getWorldPosition();
		break;

	case TS_PARENT:
		origin = m_position;
		break;

	case TS_LOCAL:
		origin = Vector3::ZERO;
		break;
	}

	setDirection(targetPoint - origin, relativeTo, localDirectionVector);
}

//-----------------------------------------------------------------------

void Transforms::setOrientation(const Quaternion& q)
{
	m_orientation = q;
	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::setOrientation(Real w, Real x, Real y, Real z)
{
	setOrientation(Quaternion(w, x, y, z));
}

//-----------------------------------------------------------------------

void Transforms::roll(const Radian& angle, tTransformSpace relativeTo)
{
	rotate(Vector3::UNIT_Z, angle, relativeTo);
}

//-----------------------------------------------------------------------

void Transforms::pitch(const Radian& angle, tTransformSpace relativeTo)
{
	rotate(Vector3::UNIT_X, angle, relativeTo);
}

//-----------------------------------------------------------------------

void Transforms::yaw(const Radian& angle, tTransformSpace relativeTo)
{
	rotate(Vector3::UNIT_Y, angle, relativeTo);
}

//-----------------------------------------------------------------------

void Transforms::rotate(const Vector3& axis, const Radian& angle,
						tTransformSpace relativeTo)
{
	Quaternion q;
	q.FromAngleAxis(angle,axis);
	rotate(q, relativeTo);
}

//-----------------------------------------------------------------------

void Transforms::rotate(const Quaternion& q, tTransformSpace relativeTo)
{
	switch (relativeTo)
	{
	case TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		m_orientation = q * m_orientation;
		break;

	case TS_WORLD:
		// Rotations are normally relative to local axes, transform up
		m_orientation = m_orientation * getWorldOrientation().Inverse() * q * getWorldOrientation();
		break;

	case TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		m_orientation = m_orientation * q;
		break;
	}

	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::resetOrientation()
{
	m_orientation = Quaternion::IDENTITY;
	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::setInheritOrientation(bool bInherit)
{
	m_bInheritOrientation = bInherit;
	needUpdate();
}

//-----------------------------------------------------------------------

Quaternion Transforms::getWorldOrientation()
{
	if (m_bDirty)
		update();

	return m_fullOrientation;
}


/**************************************** SCALE *****************************************/

void Transforms::setScale(const Vector3& scale)
{
	m_scale = scale;
	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::setScale(Real x, Real y, Real z)
{
	setScale(Vector3(x, y, z));
}

//-----------------------------------------------------------------------

void Transforms::scale(const Vector3& scale)
{
	m_scale = m_scale * scale;
	needUpdate();
}

//-----------------------------------------------------------------------

void Transforms::scale(Real x, Real y, Real z)
{
	scale(Vector3(x, y, z));
}

//-----------------------------------------------------------------------

void Transforms::setInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
	needUpdate();
}

//-----------------------------------------------------------------------

Vector3 Transforms::getWorldScale()
{
	if (m_bDirty)
		update();

	return m_fullScale;
}


/*************************************** METHODS ****************************************/

void Transforms::needUpdate()
{
	m_bDirty = true;

	m_signals.fire(SIGNAL_COMPONENT_TRANSFORMS_CHANGED);
}

//-----------------------------------------------------------------------

void Transforms::update()
{
	if (!m_bDirty)
		return;

	if (m_pTransformsOrigin)
	{
		// Update orientation
		const Quaternion& parentOrientation = m_pTransformsOrigin->getWorldOrientation();
		if (m_bInheritOrientation)
		{
			// Combine orientation with that of parent
			m_fullOrientation = parentOrientation * m_orientation;
		}
		else
		{
			// No inheritence
			m_fullOrientation = m_orientation;
		}

		// Update scale
		const Vector3& parentScale = m_pTransformsOrigin->getWorldScale();
		if (m_bInheritScale)
		{
			// Scale own position by parent scale, NB just combine
			// as equivalent axes, no shearing
			m_fullScale = parentScale * m_scale;
		}
		else
		{
			// No inheritence
			m_fullScale = m_scale;
		}

		// Change position vector based on parent's orientation & scale
		m_fullPosition = parentOrientation * (parentScale * m_position);

		// Add altered position vector to parents
		m_fullPosition += m_pTransformsOrigin->getWorldPosition();
	}
	else
	{
		// No parent
		m_fullPosition		= m_position;
		m_fullOrientation	= m_orientation;
		m_fullScale			= m_scale;
	}

	m_bDirty = false;
}


/**************************************** SLOTS *****************************************/

void Transforms::onTransformsOriginChanged(Utils::Variant* pValue)
{
	// Assertions
	assert(m_pList);
	assert(pValue);

	// Unregister to the signals of the previous origin
	if (m_pTransformsOrigin)
	{
		SignalsList* pSignals = m_pTransformsOrigin->getSignalsList();
		pSignals->disconnect(SIGNAL_COMPONENT_TRANSFORMS_CHANGED, this, &Transforms::onTransformsChanged);
	}

	Transforms* pTransforms = Transforms::cast(m_pList->getComponent(tComponentID(pValue->toString())));

	// Register to the signals of the new origin
	if (pTransforms)
	{
		SignalsList* pSignals = pTransforms->getSignalsList();
		pSignals->connect(SIGNAL_COMPONENT_TRANSFORMS_CHANGED, this, &Transforms::onTransformsChanged);
	}
}

//-----------------------------------------------------------------------

void Transforms::onTransformsChanged(Utils::Variant* pValue)
{
	needUpdate();
}


/***************************** MANAGEMENT OF THE PROPERTIES *****************************/

PropertiesList* Transforms::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Position
	pProperties->set("position", new Variant(m_position));

	// Orientation
	pProperties->set("orientation", new Variant(m_orientation));

	// Scale
	pProperties->set("scale", new Variant(m_scale));

	// Inherit orientation
	pProperties->set("inheritOrientation", new Variant(m_bInheritOrientation));

	// Inherit scale
	pProperties->set("inheritScale", new Variant(m_bInheritScale));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool Transforms::setProperty(const std::string& strCategory, const std::string& strName,
							 Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return Transforms::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool Transforms::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Position
	if (strName == "position")
		setPosition(pValue->toVector3());

	// Orientation
	else if (strName == "orientation")
		setOrientation(pValue->toQuaternion());

	// Scale
	else if (strName == "scale")
		setScale(pValue->toVector3());

	// Inherit orientation
	else if (strName == "inheritOrientation")
		setInheritOrientation(pValue->toBool());

	// Inherit scale
	else if (strName == "inheritScale")
		setInheritScale(pValue->toBool());

	// Destroy the value
	delete pValue;

	return true;
}
