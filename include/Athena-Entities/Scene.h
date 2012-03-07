/** @file	Scene.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Entities::Scene'
*/

#ifndef _ATHENA_ENTITIES_SCENE_H_
#define _ATHENA_ENTITIES_SCENE_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Entities/ComponentsList.h>
#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief	Represents a scene, ie. an object which is used to hold some environment
///			settings (in its components), like ambient light, background color, fog, sky,
///         physical world parameters... and of course, a collection of entities
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL Scene
{
	//_____ Internal types __________
public:
	typedef std::list<Entity*>					    tEntitiesList;
	typedef Utils::VectorIterator<tEntitiesList>	tEntitiesIterator;
	typedef tEntitiesList::iterator				    tEntitiesNativeIterator;


	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the scene
    //------------------------------------------------------------------------------------
	Scene(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~Scene();


	//_____ Management of the scene __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Enable/Disable the scene
    ///
    /// When a scene is disabled, none of its representation is used by anymore by the
    /// subsystems of Athena (ie. no effect in physics simulation, no update of the
    /// animations, all the entities are hidden, no sound played, ...)
	///
	/// The signals SIGNAL_SCENE_ENABLED and SIGNAL_SCENE_DISABLED are emitted.
    //------------------------------------------------------------------------------------
	void enable(bool bEnabled);

    //------------------------------------------------------------------------------------
    /// @brief	Show the scene
    ///
    /// When a scene is shown, it change all the settings (fog, ambient light, skybox,
    /// ...) to match its internal state. Its entities are also shown (it they have a
    /// visual representation).
	///
	/// The signal SIGNAL_SCENE_SHOWN is emitted.
    //------------------------------------------------------------------------------------
	void show();

    //------------------------------------------------------------------------------------
    /// @brief	Hide the scene
    ///
    /// When a scene is hidden, it change all the settings (fog, ambient light, skybox,
    /// ...) to default, neutral values. Its entities are also hidden (it they have a
    /// visual representation)..
	///
	/// The signal SIGNAL_SCENE_HIDDEN is emitted.
    //------------------------------------------------------------------------------------
	void hide();

    //------------------------------------------------------------------------------------
	/// @brief	Indicates if the scene is currently enabled
    //------------------------------------------------------------------------------------
	inline bool isEnabled() const { return m_bEnabled; }

    //------------------------------------------------------------------------------------
	/// @brief	Indicates if the scene is currently shown
    //------------------------------------------------------------------------------------
	inline bool isShown() { return m_bShown; }

    //------------------------------------------------------------------------------------
	/// @brief	Returns the name of the scene
    //------------------------------------------------------------------------------------
	inline std::string getName() const { return m_strName; }


	//_____ Management of the entities __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Create a new entity
    ///
    /// @param	strName		Name of the entity
    /// @param	pParent		Parent entity to use (0 if none)
    /// @return				The new entity, 0 if failed
    //------------------------------------------------------------------------------------
	Entity* create(const std::string& strName, Entity* pParent = 0);

    //------------------------------------------------------------------------------------
    /// @brief	Returns an entity
    ///
    /// @param	strName		Name of the entity
    /// @return				The entity, 0 if not found
    //------------------------------------------------------------------------------------
	Entity* getEntity(const std::string& strName);
	
    //------------------------------------------------------------------------------------
    /// @brief	Destroy an entity
    ///
    /// @param	strName		Name of the entity
    //------------------------------------------------------------------------------------
	void destroy(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief	Destroy an entity
    ///
    /// @param	pEntity	    The entity
    //------------------------------------------------------------------------------------
	void destroy(Entity* pEntity);

    //------------------------------------------------------------------------------------
    /// @brief	Destroy all the entities
    //------------------------------------------------------------------------------------
	void destroyAll();

    //------------------------------------------------------------------------------------
    /// @brief	Transfer an entity from another scene into this one
    ///
    /// @param	strName		Name of the entity
    /// @param	pSrcScene	Scene containing the entity
    //------------------------------------------------------------------------------------
	void transfer(const std::string& strName, Scene* pSrcScene);

    //------------------------------------------------------------------------------------
    /// @brief	Transfer an entity from another scene into this one
    ///
    /// @param	pEntity	    The entity
    //------------------------------------------------------------------------------------
	void transfer(Entity* pEntity);
	
    //------------------------------------------------------------------------------------
    /// @brief	Returns an iterator over the entities
    //------------------------------------------------------------------------------------
	inline tEntitiesIterator getEntitiesIterator()
	{
	    return tEntitiesIterator(m_entities.begin(), m_entities.end());
	}

    //------------------------------------------------------------------------------------
    /// @brief	Returns the number of entities
    //------------------------------------------------------------------------------------
	inline unsigned int getNbEntities() const
	{
	    return (unsigned int) m_entities.size();
	}


	//_____ Management of the components __________
public:
    //------------------------------------------------------------------------------------
	/// @brief	Returns the list of components of the scene
    //------------------------------------------------------------------------------------
	inline ComponentsList* getComponentsList()
	{
		return &m_components;
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns one of the components of the scene
	///
	///	@param	id	The id of the component
	/// @return		The component
    //------------------------------------------------------------------------------------
	inline Component* getComponent(const tComponentID& id) const
	{
		return m_components.getComponent(id);
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns one of the components of the scene
    ///
	///	@param	uiIndex		The index of the component
	/// @return				The component
    //------------------------------------------------------------------------------------
	inline Component* getComponent(unsigned int uiIndex) const
	{
		return m_components.getComponent(uiIndex);
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns an iterator over the components of the scene
    //------------------------------------------------------------------------------------
	inline Component::tComponentsIterator getComponentsIterator()
	{
		return m_components.getComponentsIterator();
	}

    //------------------------------------------------------------------------------------
	/// @brief	Returns the number of components of the scene
    //------------------------------------------------------------------------------------
	inline unsigned int getNbComponents() const
	{
		return m_components.getNbComponents();
	}
	
    //------------------------------------------------------------------------------------
	/// @brief	Sets the main visual, physical or audio component of the scene
	///
	/// Must only be called by those components. The goal is to provide an easy access to
	/// those components.
    //------------------------------------------------------------------------------------
    inline void _setMainComponent(Component* pComponent)
    {
        assert(pComponent->getID().type >= COMP_VISUAL);
        assert(pComponent->getID().type <= COMP_PHYSICAL);
        
        m_mainComponents[pComponent->getID().type - COMP_VISUAL] = pComponent;
    }

    //------------------------------------------------------------------------------------
	/// @brief	Reset the main component of the given type (visual, physical or audio)
	///
	/// Must only be called by those components, when destroyed
    //------------------------------------------------------------------------------------
    inline void _resetMainComponent(tComponentType type)
    {
        assert(type >= COMP_VISUAL);
        assert(type <= COMP_PHYSICAL);
        
        m_mainComponents[type - COMP_VISUAL] = 0;
    }

    //------------------------------------------------------------------------------------
	/// @brief	Returns the main component of the given type
    //------------------------------------------------------------------------------------
    inline Component* getMainComponent(tComponentType type)
    {
        assert(type >= COMP_VISUAL);
        assert(type <= COMP_PHYSICAL);
        
        return m_mainComponents[type - COMP_VISUAL];
    }


	//_____ Management of the signals list __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns the signals list used by the scene
    //------------------------------------------------------------------------------------
	inline Signals::SignalsList* getSignalsList()
	{
		return &m_signals;
	}


	//_____ Attributes __________
protected:
	std::string			    m_strName;		        ///< Name of the scene
	bool					m_bEnabled;		        ///< Indicates if the scene is enabled
	bool					m_bShown;		        ///< Indicates if the scene is shown
	Signals::SignalsList	m_signals;		        ///< The signals list
	tEntitiesList			m_entities;		        ///< The list of entities of the scene
	ComponentsList			m_components;           ///< The list of components
    Component*              m_mainComponents[3];    ///< Main visual, physical and audio components
};

}
}

#endif
