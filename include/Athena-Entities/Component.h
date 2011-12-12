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
/// By default, a component isn't affected by any transforms. Invoke the 'setTransforms()'
/// method to assign a transforms component (or use 0, to use the one of the entity owning
/// the component).
///
/// Note that any component that MUST be affected by the transformations of the entity by
/// default MUST invoke 'setTransforms(0)' in their constructor.
///
/// @remark	Components are kept in lists (see ComponentsList)
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL Component: public Utils::Describable
{
	//_____ Internal types __________
public:
	typedef std::vector<Component*>				    tComponentsList;
	typedef Utils::VectorIterator<tComponentsList>	tComponentsIterator;


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


    //_____ Management of the transformations __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Sets the Transforms of this component
	///
	/// @param	pTransforms		The transforms. If 0, the transforms component of the
	///							entity will be used.
	//-----------------------------------------------------------------------------------
    void setTransforms(Transforms* pTransforms);

	//-----------------------------------------------------------------------------------
	/// @brief	Sets the Transforms of this component
	//-----------------------------------------------------------------------------------
    void setTransforms(const tComponentID& id);

	//-----------------------------------------------------------------------------------
	/// @brief	Make this component not affected by any Transforms (the default)
	//-----------------------------------------------------------------------------------
    void removeTransforms();

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Transforms of this component
	//-----------------------------------------------------------------------------------
    inline Transforms* getTransforms() const
    {
        return m_pTransforms;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Called when the transforms affecting this component have changed
	///
	/// Can be called when the component isn't affected by any transforms anymore
	/// (getTransforms() returns 0).
	///
	/// @remark	If you override it in your component, don't forget to call the base class
	///			implementation!
	//-----------------------------------------------------------------------------------
	virtual void onTransformsChanged();


    //_____ Referers / Referees management __________
protected:
	//-----------------------------------------------------------------------------------
	/// @brief	Called when a component this one is referring to is destroyed
	///
	/// @remark	If you override it in your component, don't forget to call the base class
	///			implementation!
	//-----------------------------------------------------------------------------------
	virtual void onComponentDestroyed(Component* pReferee);


    inline void addReferer(Component* pReferer)
    {
        assert(pReferer);
        m_referers.push_back(pReferer);
    }

    inline void removeReferer(Component* pReferer)
    {
        assert(pReferer);
        
        tComponentsList::iterator iter, iterEnd;
        for (iter = m_referers.begin(), iterEnd = m_referers.end(); iter != iterEnd; ++iter)
        {
            if (*iter == pReferer)
            {
                m_referers.erase(iter);
                return;
            }
        }
    }

    inline void addReferee(Component* pReferee)
    {
        assert(pReferee);
        m_referees.push_back(pReferee);
    }

    inline void removeReferee(Component* pReferee)
    {
        assert(pReferee);
        
        tComponentsList::iterator iter, iterEnd;
        for (iter = m_referees.begin(), iterEnd = m_referees.end(); iter != iterEnd; ++iter)
        {
            if (*iter == pReferee)
            {
                m_referees.erase(iter);
                return;
            }
        }
    }


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
	static const std::string	TYPE;		///< Type of component


	//_____ Attributes __________
protected:
	tComponentID			m_id;			///< ID of the component
	ComponentsList*		    m_pList;		///< The list containing that component
	tComponentsList	        m_referers;	    ///< The list of components that refers to this one
	tComponentsList	        m_referees;	    ///< The list of components refered to by this one
	Transforms*			    m_pTransforms;	///< The transforms origin
	Signals::SignalsList	m_signals;		///< The signals list
};

}
}

#endif
