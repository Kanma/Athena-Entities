/** @file	Component.h
	@author	Philip Abbet

	Definition of the class 'Athena::Entities::Component'
*/

#ifndef _ATHENA_ENTITIES_COMPONENT_H_
#define _ATHENA_ENTITIES_COMPONENT_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/tComponentID.h>
#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Signals/SignalsList.h>


namespace Athena {
namespace Entities {

//----------------------------------------------------------------------------------------
/// @brief	Base class for all the components of an entity
///
/// @remark	Components are kept in lists (see ComponentsList)
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL Component: public Utils::Describable
{
	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    ///
    /// @param	strName	Name of the component
    /// @param	pList	Components list to which the component must be added
    //------------------------------------------------------------------------------------
	Component(const std::string& strName, ComponentsList* pList);

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	virtual ~Component();

    //------------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	Components list to which the component must be added
    /// @return			The new component
    //------------------------------------------------------------------------------------
	static Component* create(const std::string& strName, ComponentsList* pList);


	//_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the component
	//-----------------------------------------------------------------------------------
	virtual const std::string getType() const { return TYPE; }


	//-----------------------------------------------------------------------------------
	/// @brief	Returns the id of the component
	//-----------------------------------------------------------------------------------
	tComponentID getID() const { return m_id; }


	//-----------------------------------------------------------------------------------
	/// @brief	Returns the name of the component
	//-----------------------------------------------------------------------------------
	inline const std::string& getName() const { return m_id.strName; }


	//-----------------------------------------------------------------------------------
	/// @brief	Returns the list of which the component is a member
	//-----------------------------------------------------------------------------------
	inline ComponentsList* getList() const { return m_pList; }


    //_____ Management of the origin of the transformations __________
public:
    void setTransformsOrigin(Transforms* pTransforms);
    void setTransformsOrigin(const tComponentID& id);

    Transforms* getTransformsOrigin() const
    {
        return m_pTransformsOrigin;
    }


    //_____ Slots __________
protected:
    void onTransformsOriginDestroyed(Utils::Variant* pValue);


	//_____ Management of the signals list __________
public:
	//------------------------------------------------------------------------------------
	/// @brief	Returns the signals list of the component
	//------------------------------------------------------------------------------------
	inline Signals::SignalsList* getSignalsList()
	{
		return &m_signals;
	}


	//_____ Management of the properties __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns a list containing the properties of the component
    ///
    /// Used in the serialization mecanism of the components
    ///
    /// @remark	Must be overriden by each component type. Each implementation must first
    ///         call the one of its base class, and add a new category (named after the
    ///         component's type) AT THE BEGINNING of the obtained list, containing the
    ///         properties related to this type.
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
    ///			process the property's value. Otherwise, it must call the implementation
    ///         of its base class.
    //------------------------------------------------------------------------------------
	virtual bool setProperty(const std::string& strCategory, const std::string& strName,
							 Utils::Variant* pValue);
							 
    //------------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the components
    ///
    ///	@param	strName		The name of the property
    ///	@param	pValue		The value of the property
    ///	@return				'true' if the property was used, 'false' if a required object
    ///						is missing
    //------------------------------------------------------------------------------------
	bool setProperty(const std::string& strName, Utils::Variant* pValue);


	//_____ Constants __________
public:
	static const std::string	TYPE;				///< Type of component


	//_____ Attributes __________
protected:
	tComponentID			m_id;					///< ID of the component
	ComponentsList*		    m_pList;				///< The list containing that component
	Transforms*			    m_pTransformsOrigin;	///< The transforms origin
	Signals::SignalsList	m_signals;				///< The signals list
};

}
}

#endif
