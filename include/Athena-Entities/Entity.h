/** @file	Entity.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Entities::Entity'
*/

#ifndef _ATHENA_ENTITIES_ENTITY_H_
#define _ATHENA_ENTITIES_ENTITY_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {


//---------------------------------------------------------------------------------------
/// @brief	Represents a game entity
///
/// A game entity has:
///   - a transforms component
///   - a signals list
///
/// It can also have:
///   - a parent entity
///   - some children
///   - some components
///   - some animations (through an AnimationsMixer, @see Animation::CAnimationsMixer)
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL Entity
{
	friend class Scene;


	//_____ Internal types __________
public:
    typedef std::list<Entity*>                      tEntitiesList;
	typedef Utils::VectorIterator<tEntitiesList>	ChildrenIterator;
    typedef tEntitiesList::iterator                 tEntitiesNativeIterator;


	//_____ Construction / Destruction __________
protected:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    ///
    /// @param	strName		Name of the entity
    /// @param	pParent		Parent entity, 0 if none
    ///
    /// @remark Entities can only be created by a scene
    //------------------------------------------------------------------------------------
	Entity(const std::string& strName, Scene* pScene, Entity* pParent = 0);

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	virtual ~Entity();


	//_____ Management of the entity __________
public:
    //------------------------------------------------------------------------------------
	/// @brief	Returns the name of the entity
    //------------------------------------------------------------------------------------
	inline const std::string& getName() const { return m_strName; }

    //------------------------------------------------------------------------------------
	/// @brief	Returns the scene containing the entity
    //------------------------------------------------------------------------------------
	inline Scene* getScene() { return m_pScene; }

    //------------------------------------------------------------------------------------
	/// @brief	Enable/Disable the entity
	///
	/// It's up to the components to determine what to do when the entity is disabled, but
	/// generally nothing should be updated (no more animation, no more sound, ...)
	///
	/// The signals SIGNAL_ENTITY_ENABLED and SIGNAL_ENTITY_DISABLED are emitted to
	/// inform the components about the state of the entity.
    //------------------------------------------------------------------------------------
	void enable(bool bEnabled);

    //------------------------------------------------------------------------------------
	/// @brief	Indicates if the entity is currently enabled
    //------------------------------------------------------------------------------------
	inline bool isEnabled() const { return m_bEnabled; }


	//_____ Management of the parent/children relation __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Add a child to the entity
    ///
    /// @param	pChild	The new child
    //------------------------------------------------------------------------------------
	void addChild(Entity* pChild);

    //------------------------------------------------------------------------------------
    /// @brief	Remove a child from the entity
    ///
    /// @param	pChild	The child to remove
    ///
    /// @remark	The child isn't destroyed by this operation. It becomes a 'root entity' of
    ///         the scene
    //------------------------------------------------------------------------------------
	void removeChild(Entity* pChild);

    //------------------------------------------------------------------------------------
    /// @brief	Destroy all the children of the entity
    ///
    /// @remark	The children and all their children are destroyed by this operation
    //------------------------------------------------------------------------------------
	void destroyAllChildren();

    //------------------------------------------------------------------------------------
	/// @brief	Returns the parent of the entity
	/// @return	The parent entity, 0 if none
    //------------------------------------------------------------------------------------
	inline Entity* getParent() const
	{
	    return m_pParent;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the number of children of the entity
    //------------------------------------------------------------------------------------
	inline unsigned int getNbChildren() const
	{
	    return (unsigned int) m_children.size();
	}
	
    //------------------------------------------------------------------------------------
	/// @brief	Returns an iterator over the children of the entity
    //------------------------------------------------------------------------------------
	ChildrenIterator getChildrenIterator()
	{
	    return ChildrenIterator(m_children.begin(), m_children.end());
	}


	//_____ Management of the components __________
public:
    //------------------------------------------------------------------------------------
	/// @brief	Returns the list of components of the entity
    //------------------------------------------------------------------------------------
	inline ComponentsList* getComponentsList()
	{
		return &m_components;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns one of the components of the entity
	///
	///	@param	id	The id of the component
	/// @return		The component
    //------------------------------------------------------------------------------------
	inline Component* getComponent(const tComponentID& id) const
	{
		return m_components.getComponent(id);
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns one of the components of the entity
	///
	///	@param	uiIndex		The index of the component
	/// @return				The component
    //------------------------------------------------------------------------------------
	inline Component* getComponent(unsigned int uiIndex) const
	{
		return m_components.getComponent(uiIndex);
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns an iterator over the components of the entity
    //------------------------------------------------------------------------------------
	inline ComponentsList::tComponentsIterator getComponentsIterator()
	{
		return m_components.getComponentsIterator();
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the number of components of the entity
    //------------------------------------------------------------------------------------
	inline unsigned int getNbComponents() const
	{
		return m_components.getNbComponents();
	}
    

	//_____ Management of the signals list __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns the signals list used by the entity
    //------------------------------------------------------------------------------------
	inline Signals::SignalsList* getSignalsList()
	{
		return &m_signals;
	}


	//_____ Management of the transforms __________
public:
    inline Transforms* getTransforms()
    {
        return m_pTransforms;
    }


	//_____ Management of the animations __________
// public:
//  Animation::AnimationsMixer* createAnimationsMixer();
// 
//  inline Animation::CAnimationsMixer* getAnimationsMixer()
//  {
//      return m_pAnimationsMixer;
//  }


	//_____ Attributes __________
protected:
	std::string						m_strName;			///< Name
	Scene*							m_pScene;			///< The scene managing the entity
	bool							m_bEnabled;			///< Indicates if the entity is enabled
														///  used by this entity
	ComponentsList					m_components;		///< The list of components
	Signals::SignalsList			m_signals;			///< The signals list
    // Animation::AnimationsMixer*  m_pAnimationsMixer; ///< The animations mixer
    Transforms*                     m_pTransforms;      ///< The transforms

	// Parent/children relations
	Entity*							m_pParent;			///< Parent of this entity
	tEntitiesList					m_children;			///< Children of the entity
};

}
}

#endif
