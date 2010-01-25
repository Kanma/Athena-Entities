/** @file	ComponentsList.h
	@author	Philip Abbet

	Definition of the class 'Athena::Entities::ComponentsList'
*/

#ifndef _ATHENA_ENTITIES_COMPONENTSLIST_H_
#define _ATHENA_ENTITIES_COMPONENTSLIST_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/Component.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief	Represents a list of components
///
/// @remark	A list can be associated with an entity or with a scene
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL ComponentsList
{
	//_____ Internal types __________
public:
	typedef std::vector<Component*>				    tComponentsList;
	typedef Utils::VectorIterator<tComponentsList>	tComponentsIterator;


	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    //------------------------------------------------------------------------------------
	ComponentsList();

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~ComponentsList();


	//_____ Methods __________
public:
    //------------------------------------------------------------------------------------
	/// @brief	Informs the list about the entity to which it belongs
	/// @param	pEntity		The entity to which that list belongs
    //------------------------------------------------------------------------------------
	inline void _setEntity(Entity* pEntity)
	{
		m_pEntity = pEntity;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the entity to which that list belongs
	/// @return	The entity
    //------------------------------------------------------------------------------------
	inline Entity* getEntity() const
	{
		return m_pEntity;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Informs the list about the scene to which it belongs
	/// @param	pScene		The scene to which that list belongs
    //------------------------------------------------------------------------------------
	inline void _setScene(Scene* pScene)
	{
		m_pScene = pScene;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the scene to which that list belongs
	/// @return	The scene
    //------------------------------------------------------------------------------------
	inline Scene* getScene() const
	{
		return m_pScene;
	}

    //------------------------------------------------------------------------------------
    /// @brief	Add a component to the list
    ///
    /// @param	pComponent	The component
    /// @return				'true' if successful
    //------------------------------------------------------------------------------------
	bool _addComponent(Component* pComponent);

    //------------------------------------------------------------------------------------
    /// @brief	Remove a component from the list
    ///
    /// @param	pComponent	The component
    /// @param	bDestroy	Indicates if the component must also be destroyed
    //------------------------------------------------------------------------------------
	void removeComponent(Component* pComponent, bool bDestroy = true);

    //------------------------------------------------------------------------------------
    /// @brief	Remove a component from the list
    ///
    /// @param	id			ID of the component
    /// @param	bDestroy	Indicates if the component must also be destroyed
    //------------------------------------------------------------------------------------
	void removeComponent(const tComponentID& id, bool bDestroy = true);

    //------------------------------------------------------------------------------------
    /// @brief	Remove all the components from the list
    ///
    /// @param	bDestroy	Indicates if the components must also be destroyed
    //------------------------------------------------------------------------------------
	void removeAllComponents(bool bDestroy = true);

    //------------------------------------------------------------------------------------
    /// @brief	Returns one of the components
    ///
    /// @param	id	ID of the component
    /// @return		The component
    ///
    ///	@remark	If the component doesn't belongs to the entity owning the list (if any), and
    ///			it is a transforms component, the parent entity (if any) is asked for the
    ///			component.
    //------------------------------------------------------------------------------------
	Component* getComponent(const tComponentID& id) const;

    //------------------------------------------------------------------------------------
	/// @brief	Returns one of the components
	///
	///	@param	uiIndex		The index of the component
	/// @return	The component
    //------------------------------------------------------------------------------------
	inline Component* getComponent(unsigned int uiIndex) const
	{
		assert(uiIndex < getNbComponents());
		return m_components[uiIndex];
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns an iterator over the components of the list
    //------------------------------------------------------------------------------------
	inline tComponentsIterator getComponentsIterator()
	{
		return tComponentsIterator(m_components.begin(), m_components.end());
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the number of components in the list
    //------------------------------------------------------------------------------------
	inline unsigned int getNbComponents() const
	{
		return (unsigned int) m_components.size();
	}


	//_____ Attributes __________
private:
	Entity*		    m_pEntity;		///< The entity associated with this list
	Scene*		    m_pScene;		///< The scene associated with this list
	tComponentsList	m_components;	///< The components
};

}
}

#endif
