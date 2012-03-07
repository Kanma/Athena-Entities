/** @file   ComponentsManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Entities::ComponentsManager'
*/

#ifndef _ATHENA_ENTITIES_COMPONENTSMANAGER_H_
#define _ATHENA_ENTITIES_COMPONENTSMANAGER_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {

//----------------------------------------------------------------------------------------
/// @brief  Type of the 'Component creation method' to be registered with the
///         Components manager
//----------------------------------------------------------------------------------------
typedef Component* ComponentCreationMethod(const std::string&, ComponentsList*);


//----------------------------------------------------------------------------------------
/// @brief  Represents the manager of the composants of an entity (or scenes)
///
/// This manager is responsible of the creation/destruction of the components
/// of the entities.
///
/// There is two ways to create a component: instanciate an object of the desired class
/// (on the stack!) OR ask the manager to create the component.
///
/// To do that, it is necessary to specify the type of the component. Each type of
/// component is associated with a 'creation method'. The Components manager call this
/// method when it needs to create a component of its associated type.
///
/// This mecanism is mainly targetted at scene loaders that read the type of the components
/// to create from a file.
///
/// Other modules of Athena defines some component types. The user can register new types
/// by subclassing an existing component class, and registering a 'component creation
/// method' with the manager.
///
/// The components are associated with a 'components list', which hold all the components
/// of an entity (or scene). The destruction of the components is handled by those lists.
///
/// @remark This class is a singleton.
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL ComponentsManager: public Utils::Singleton<ComponentsManager>
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    ComponentsManager();

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    ~ComponentsManager();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a reference to the instance of the components manager
    /// @return A reference to the manager
    //------------------------------------------------------------------------------------
    static ComponentsManager& getSingleton();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a pointer to the instance of the components manager
    /// @return A pointer to the manager
    //------------------------------------------------------------------------------------
    static ComponentsManager* getSingletonPtr();


    //_____ Management of the components __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Create a new component
    ///
    /// @param  strType Type of the component
    /// @param  strName Name of the component
    /// @param  pList   List to attach the component to
    /// @return         The new component, 0 if failed
    //------------------------------------------------------------------------------------
    Component* create(const std::string& strType, const std::string& strName,
                      ComponentsList* pList);

    //------------------------------------------------------------------------------------
    /// @brief  Destroy a component
    ///
    /// @param  pComponent  The component
    //------------------------------------------------------------------------------------
    void destroy(Component* pComponent);


    //_____ Registration of new types of components __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Register a new type of component, with a C++ 'component creation method'
    ///
    /// @param  strType         Name of the type
    /// @param  pCreationMethod The creation method
    //------------------------------------------------------------------------------------
    void registerType(const std::string& strType, ComponentCreationMethod* pCreationMethod);

// #if ATHENA_ENTITIES_SCRIPTING
//
//     //------------------------------------------------------------------------------------
//     /// @brief   Register a new type of component, with a Python 'component creation
//     ///         function'
//     ///
//     /// @param   strType     Name of the type
//     /// @param   strClass    The Python class
//     //------------------------------------------------------------------------------------
//  void registerType(const std::string& strType, const std::string& strClass);
//
//     //------------------------------------------------------------------------------------
//  /// @brief  Notify the manager about the Python creator to use
//     ///
//  /// @param  pCreator    The creator
//     ///
//     /// @remark We don't care about destruction: the creator is expected to be a Python
//     ///         object which will be garbage collected at adapted time
//     //------------------------------------------------------------------------------------
//  inline void _setCreator(IComponentsCreator* pCreator)
//  {
//      // We don't care about destruction: the creator is expected to be a Python object
//      // which will be garbage collected at adapted time
//      m_pCreator = pCreator;
//  }
//
//     //------------------------------------------------------------------------------------
//  /// @brief  Returns the Python creator used by the manager
//     //------------------------------------------------------------------------------------
//  inline IComponentsCreator* getCreator() const
//  {
//      return m_pCreator;
//  }
//
// #endif


    //_____ Internal types __________
private:

// #if ATHENA_ENTITIES_SCRIPTING
//
//     //------------------------------------------------------------------------------------
//  /// @brief  Contains the informations necessary to create a new component of a
//  ///         specific type
//     //------------------------------------------------------------------------------------
//  struct tCreationInfos
//  {
//      bool                        bPointer;   ///< Indicates if the 'component creation
//                                              ///  method' is a C++ or a Python one
//      ComponentCreationMethod*    pMethod;    ///< The C++ creation method
//      std::string                 strClass;   ///< The Python class
//  };
//
//     typedef std::map<std::string, tCreationInfos> tCreationsInfosList;
//
// #else
//
    typedef std::map<std::string, ComponentCreationMethod*> tCreationsInfosList;

// #endif

    typedef Utils::MapIterator<tCreationsInfosList> tCreationsInfosIterator;
    typedef tCreationsInfosList::iterator           tCreationsInfosNativeIterator;


    //_____ Attributes __________
private:
    tCreationsInfosList m_types;    ///< The registered types

// #if ATHENA_ENTITIES_SCRIPTING
//  IComponentsCreator* m_pCreator; ///< The Python creator object to use
// #endif
};

}
}

#endif
