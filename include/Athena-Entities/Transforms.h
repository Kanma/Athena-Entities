/** @file	Transforms.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Entities::Transforms'

    @note   This is based on the 'Ogre::SceneNode' class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#ifndef _ATHENA_ENTITIES_TRANSFORMS_H_
#define _ATHENA_ENTITIES_TRANSFORMS_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief	Component containing transforms
///
/// Components of this kind can be used as transforms origin for other components. The
/// transforms are relative to the transforms origin of the component, unless stated
/// otherwise.
///
/// Each entity contains at least one transforms component.
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL Transforms: public Component
{
    friend class Entity;
    

	//_____ Internal types __________
public:
	//------------------------------------------------------------------------------------
	///	@brief	Enumeration denoting the spaces which a transform can be relative to
	//------------------------------------------------------------------------------------
	enum tTransformSpace
	{
		TS_LOCAL,		///< Transform is relative to the local space
		TS_PARENT,      ///< Transform is relative to the space of the origin
		TS_WORLD		///< Transform is relative to world space
	};


	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    ///
    /// @param	strName		Name of the component
    ///	@param	pList		List to which this component belongs
    //------------------------------------------------------------------------------------
	Transforms(const std::string& strName, ComponentsList* pList);

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	virtual ~Transforms();

    //------------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //------------------------------------------------------------------------------------
	static Transforms* create(const std::string& strName, ComponentsList* pList);

    //------------------------------------------------------------------------------------
    /// @brief	Cast a component to a Transforms
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a Transforms
    //------------------------------------------------------------------------------------
	static Transforms* cast(Component* pComponent);


	//_____ Implementation of Component __________
public:
    //------------------------------------------------------------------------------------
	/// @brief	Returns the type of the component
    //------------------------------------------------------------------------------------
	virtual const std::string getType() const { return TYPE; }


	//_____ Position __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Sets the position of the component relative to it's origin
    ///
    /// @param	pos		The position vector
    //------------------------------------------------------------------------------------
	void setPosition(const Math::Vector3& pos);

    //------------------------------------------------------------------------------------
    /// @brief	Sets the position of the component relative to it's origin
    ///
    /// @param	x	The position on the x-axis
    /// @param	y	The position on the y-axis
    /// @param	z	The position on the z-axis
    //------------------------------------------------------------------------------------
	void setPosition(Math::Real x, Math::Real y, Math::Real z);

    //------------------------------------------------------------------------------------
    /// @brief	Moves the component along the cartesian axes
    ///
    /// @param	d			Vector with x,y,z values representing the translation
    /// @param	relativeTo	The space which this transform is relative to
    //------------------------------------------------------------------------------------
	void translate(const Math::Vector3& d, tTransformSpace relativeTo = TS_LOCAL);

    //------------------------------------------------------------------------------------
    /// @brief	Moves the component along the cartesian axes
    ///
    /// @param	x, y, z		Values representing the translation
    /// @param	relativeTo	The space which this transform is relative to
    //------------------------------------------------------------------------------------
	void translate(Math::Real x, Math::Real y, Math::Real z,
	               tTransformSpace relativeTo = TS_LOCAL);

    //------------------------------------------------------------------------------------
    /// @brief	Gets the world-space position of the component
    //------------------------------------------------------------------------------------
	Math::Vector3 getWorldPosition();

    //------------------------------------------------------------------------------------
	/// @brief	Gets the position of the component relative to it's origin
    //------------------------------------------------------------------------------------
	inline Math::Vector3 getPosition() const
	{
		return m_position;
	}


	//_____ Orientation __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Sets the component's direction vector (ie it's local -Z)
    ///
    /// @remark	Note that the 'up' vector for the orientation will automatically be
    ///			recalculated based on the current 'up' vector (i.e. the roll will remain
    ///			the same)
    ///
    /// @param	vec						The direction vector
    /// @param	relativeTo				The space in which this direction vector is expressed
    /// @param	localDirectionVector	The vector which normally describes the natural
    ///									direction of the component, usually -Z
    //------------------------------------------------------------------------------------
	void setDirection(const Math::Vector3& vec, tTransformSpace relativeTo = TS_LOCAL,
					  const Math::Vector3& localDirectionVector = Math::Vector3::NEGATIVE_UNIT_Z);

    //------------------------------------------------------------------------------------
    /// @brief	Points the local -Z direction of this component at a point in space
    ///
    /// @param	targetPoint				The look at point
    /// @param	relativeTo				The space in which the point is expressed
    /// @param	localDirectionVector	The vector which normally describes the natural
    ///									direction of the component, usually -Z
    //------------------------------------------------------------------------------------
	void lookAt(const Math::Vector3& targetPoint, tTransformSpace relativeTo = TS_LOCAL,
				const Math::Vector3& localDirectionVector = Math::Vector3::NEGATIVE_UNIT_Z);

    //------------------------------------------------------------------------------------
    /// @brief	Sets the orientation of this component via a quaternion
    ///
    /// @param	q	The orientation quaternion
    //------------------------------------------------------------------------------------
	void setOrientation(const Math::Quaternion& q);

    //------------------------------------------------------------------------------------
    /// @brief	Sets the orientation of this component via quaternion parameters
    ///
    /// @param	w	The w parameter
    /// @param	x	The x parameter
    /// @param	y	The y parameter
    /// @param	z	The z parameter
    //------------------------------------------------------------------------------------
	void setOrientation(Math::Real w, Math::Real x, Math::Real y, Math::Real z);

    //------------------------------------------------------------------------------------
    /// @brief	Rotate the component around the Z-axis
    ///
    /// @param	angle		Angle of the rotation
    /// @param	relativeTo	The space which this transform is relative to
    //------------------------------------------------------------------------------------
	void roll(const Math::Radian& angle, tTransformSpace relativeTo = TS_LOCAL);

    //------------------------------------------------------------------------------------
    /// @brief	Rotate the component around the X-axis
    ///
    /// @param	angle		Angle of the rotation
    /// @param	relativeTo	The space which this transform is relative to
    //------------------------------------------------------------------------------------
	void pitch(const Math::Radian& angle, tTransformSpace relativeTo = TS_LOCAL);
	
    //------------------------------------------------------------------------------------
    /// @brief	Rotate the component around the Y-axis
    ///
    /// @param	angle		Angle of the rotation
    /// @param	relativeTo	The space which this transform is relative to
    //------------------------------------------------------------------------------------
	void yaw(const Math::Radian& angle, tTransformSpace relativeTo = TS_LOCAL);

    //------------------------------------------------------------------------------------
    /// @brief	Rotate the component around an arbitrary axis
    ///
    /// @param	axis		The axis around which the rotation will occur
    /// @param	angle		The angle
    /// @param	relativeTo	The space in which the rotation axis is expressed
    //------------------------------------------------------------------------------------
	void rotate(const Math::Vector3& axis, const Math::Radian& angle,
				tTransformSpace relativeTo = TS_LOCAL);
				
    //------------------------------------------------------------------------------------
    /// @brief	Rotate the component around an arbitrary axis using a quarternion
    ///
    /// @param	q			The quarternion representing the rotation
    /// @param	relativeTo	The space in which the rotation axis is expressed
    //------------------------------------------------------------------------------------
	void rotate(const Math::Quaternion& q, tTransformSpace relativeTo = TS_LOCAL);

    //------------------------------------------------------------------------------------
    /// @brief	Resets the component's orientation (local axes as world axes, no rotation)
    //------------------------------------------------------------------------------------
	void resetOrientation();

    //------------------------------------------------------------------------------------
    /// @brief	Tells the component whether it should inherit orientation from it's parent
    ///
    /// @remark Orientatings, unlike other transforms, are not always inherited by child
    ///			component. Whether or not orientatings affect the orientation of the
    ///			child components depends on the setInheritOrientation option of the child.
    ///			In some cases you want a orientating of a parent component to apply to a
    ///			child component (e.g. where the child component is a component of the same
    ///			object, so you want it to be the same relative orientation based on the
    ///			parent's orientation), but not in other cases (e.g. where the child
    ///			component is just for positioning another object, you want it to maintain
    ///			it's own orientation). The default is to inherit as with other transforms.
    ///
    /// @param	bInherit	If true, this component's orientation will be affected by its
    ///						parent's orientation. If false, it will not be affected.  
    //------------------------------------------------------------------------------------
	void setInheritOrientation(bool bInherit);

    //------------------------------------------------------------------------------------
    /// @brief	Gets the worldspace orientation of the component
    //------------------------------------------------------------------------------------
	Math::Quaternion getWorldOrientation();

    //------------------------------------------------------------------------------------
	/// @brief	Returns a quaternion representing the component's orientation relative to
	///			it's origin
    //------------------------------------------------------------------------------------
	inline Math::Quaternion getOrientation() const
	{
		return m_orientation;
	}
 

	//_____ Scale __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Sets the scaling factor applied to this component
    ///
    /// @remark Scaling factors, unlike other transforms, are not always inherited by
    ///			child component. Whether or not scalings affect both the size and position
    ///			of the child components depends on the setInheritScale option of the child.
    ///			In some cases you want a scaling factor of a parent component to apply to
    ///			a child component (e.g. where the child component is a component of the
    ///			same object, so you want it to be the same relative size and position
    ///			based on the parent's size), but not in other cases (e.g. where the child
    ///			component is just for positioning another object, you want it to maintain
    ///			it's own size and relative position). The default is to inherit as with
    ///			other transforms. Note that like rotations, scalings are oriented around
    ///         the component's origin.
    ///
    /// @param	scale	The scaling factor
    //------------------------------------------------------------------------------------
	void setScale(const Math::Vector3& scale);
	
    //------------------------------------------------------------------------------------
    /// @brief	Sets the scaling factor applied to this component
    ///
    /// @remark Scaling factors, unlike other transforms, are not always inherited by
    ///			child component. Whether or not scalings affect both the size and position
    ///			of the child components depends on the setInheritScale option of the child.
    ///			In some cases you want a scaling factor of a parent component to apply to
    ///			a child component (e.g. where the child component is a component of the
    ///			same object, so you want it to be the same relative size and position
    ///			based on the parent's size), but not in other cases (e.g. where the child
    ///			component is just for positioning another object, you want it to maintain
    ///			it's own size and relative position). The default is to inherit as with
    ///			other transforms. Note that like rotations, scalings are oriented around
    ///         the component's origin.
    ///
    /// @param	x	The scaling factor on the x-axis
    /// @param	y	The scaling factor on the y-axis
    /// @param	z	The scaling factor on the z-axis
    //------------------------------------------------------------------------------------
	void setScale(Math::Real x, Math::Real y, Math::Real z);
 
    //------------------------------------------------------------------------------------
    /// @brief	Scales the component, combining it's current scale with the passed in
    ///			scaling factor
    ///
    /// @remark This method applies an extra scaling factor to the component's existing
    ///			scale, (unlike setScale which overwrites it) combining it's current scale
    ///			with the new one. E.g. calling this method twice with Vector3(2,2,2) would
    ///			have the same effect as setScale(Vector3(4,4,4)) if the existing scale was
    ///         1. Note that like rotations, scalings are oriented around the component's
    ///         origin.
    ///
    /// @param	scale	The scaling factor
    //------------------------------------------------------------------------------------
	void scale(const Math::Vector3& scale);

    //------------------------------------------------------------------------------------
    /// @brief	Scales the component, combining it's current scale with the passed in
    ///			scaling factor
    ///
    /// @remark This method applies an extra scaling factor to the component's existing
    ///			scale, (unlike setScale which overwrites it) combining it's current scale
    ///			with the new one. E.g. calling this method twice with Vector3(2,2,2) would
    ///			have the same effect as setScale(Vector3(4,4,4)) if the existing scale was
    ///         1. Note that like rotations, scalings are oriented around the component's
    ///         origin.
    ///
    /// @param	x	The scaling factor on the x-axis
    /// @param	y	The scaling factor on the y-axis
    /// @param	z	The scaling factor on the z-axis
    //------------------------------------------------------------------------------------
	void scale(Math::Real x, Math::Real y, Math::Real z);

    //------------------------------------------------------------------------------------
    /// @brief	Tells the component whether it should inherit scaling factors from it's
    ///         parent
    ///
    /// @remark Scaling factors, unlike other transforms, are not always inherited by
    ///			child component. Whether or not scalings affect both the size and position
    ///			of the child components depends on the setInheritScale option of the child.
    ///			In some cases you want a scaling factor of a parent component to apply to
    ///			a child component (e.g. where the child component is a component of the
    ///			same object, so you want it to be the same relative size and position
    ///			based on the parent's size), but not in other cases (e.g. where the child
    ///			component is just for positioning another object, you want it to maintain
    ///			it's own size and relative position). The default is to inherit as with
    ///			other transforms. Note that like rotations, scalings are oriented around
    ///         the component's origin.
    ///
    /// @param	bInherit	If true, this component's scale and position will be affected
    ///						by its parent's scale. If false, it will not be affected.  
    //------------------------------------------------------------------------------------
	void setInheritScale(bool bInherit);
	
    //------------------------------------------------------------------------------------
    /// @brief	Gets the worldspace scale of the component
    //------------------------------------------------------------------------------------
	Math::Vector3 getWorldScale();

    //------------------------------------------------------------------------------------
	/// @brief	Gets the scaling factor of this component, relative to its origin
    //------------------------------------------------------------------------------------
	inline Math::Vector3 getScale() const
	{
		return m_scale;
	}


	//_____ Methods __________
protected:
	void needUpdate();
	void update();
	
	//-----------------------------------------------------------------------------------
	/// @brief	Called when the transforms affecting this component have changed
	///
	/// Can be called when the component isn't affected by any transforms anymore
	/// (getTransforms() returns 0).
	//-----------------------------------------------------------------------------------
	virtual void onTransformsChanged();


	//_____ Management of the properties __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns a list containing the properties of the component
    ///
    /// Used in the serialization mecanism of the components
    ///
    /// @remark	Must be overriden by each component type. Each implementation must first
    ///			call its base class one, and add a new category (named after the
    ///			component's type) AT THE BEGINNING of the obtained list, containing the
    ///			properties related to this type.
    ///
    /// @return	The list of properties
    //------------------------------------------------------------------------------------
	virtual Utils::PropertiesList* getProperties() const;
	
    //------------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the components
    ///
    ///	@param	strCategory		The category of the property
    ///	@param	strName			The name of the property
    ///	@param	pValue			The value of the property
    ///	@return					'true' if the property was used, 'false' if a required
    ///							object is missing
    ///
    /// @remark	Must be overriden by each component type. Each implementation must test
    ///			if the property's category is the one of the component's type, and if so
    ///			process the property's value. Otherwise, it must call its base class
    ///         implementation.
    //------------------------------------------------------------------------------------
	virtual bool setProperty(const std::string& strCategory, const std::string& strName,
							 Utils::Variant* pValue);

    //------------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the components
    ///	@param	strName		The name of the property
    ///	@param	pValue		The value of the property
    ///	@return				'true' if the property was used, 'false' if a required object
    ///						is missing
    //------------------------------------------------------------------------------------
	bool setProperty(const std::string& strName, Utils::Variant* pValue);


	//_____ Constants __________
public:
	static const std::string TYPE;  ///< Name of the type of component


	//_____ Attributes __________
protected:
	// Relative transforms
	Math::Vector3		m_position;
	Math::Quaternion	m_orientation;
	Math::Vector3		m_scale;

	// Full (world) transforms
	Math::Vector3		m_fullPosition;
	Math::Quaternion	m_fullOrientation;
	Math::Vector3		m_fullScale;

	bool				m_bDirty;
	unsigned int		m_uiNbFullCalculationRequests;
	bool				m_bInheritOrientation;
	bool				m_bInheritScale;
};

}
}

#endif
