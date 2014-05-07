/** @file   ComponentsManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Entities::ComponentsManager'
*/

#ifndef _ATHENA_ENTITIES_COMPONENTSMANAGER_H_
#define _ATHENA_ENTITIES_COMPONENTSMANAGER_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>
#include <Athena-Core/Log/Declarations.h>
#include <Athena-Core/Log/LogManager.h>

#if ATHENA_ENTITIES_SCRIPTING
    #include <v8.h>
#endif


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
/// This mechanism is mainly targetted at scene loaders that read the type of the
/// components to create from a file.
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
    //_____ Internal types __________
private:
    struct ComponentCreationInfos
    {
        virtual ~ComponentCreationInfos() {}

        virtual Component* create(const std::string& strName, ComponentsList* pList) = 0;

#if ATHENA_ENTITIES_SCRIPTING
        virtual v8::Handle<v8::Value> convertToJavaScript(Component* pComponent) = 0;
#endif
    };

    template<class TYPE>
    struct TemplatedComponentCreationInfos: public ComponentCreationInfos
    {
        virtual Component* create(const std::string& strName, ComponentsList* pList)
        {
            return TYPE::create(strName, pList);
        }

#if ATHENA_ENTITIES_SCRIPTING
        virtual v8::Handle<v8::Value> convertToJavaScript(Component* pComponent)
        {
            return toJavaScript(dynamic_cast<TYPE*>(pComponent));
        }
#endif
    };

    typedef std::map<std::string, ComponentCreationInfos*>  tCreationsInfosList;
    typedef Utils::MapIterator<tCreationsInfosList>         tCreationsInfosIterator;
    typedef tCreationsInfosList::iterator                   tCreationsInfosNativeIterator;


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


#if ATHENA_ENTITIES_SCRIPTING
    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript component around a C++ one, by taking its actual
    ///         type into account
    ///
    /// For instance, if the component is a Transforms one, the new JavaScript component
    /// will have all the methods and attributes of a Transforms component.
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> convertToJavaScript(Component* pComponent);
#endif


    //_____ Registration of new types of components __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Register a new type of component, with a C++ 'component creation method'
    ///
    /// @param  strType         Name of the type
    /// @param  pCreationMethod The creation method
    //------------------------------------------------------------------------------------
    template<class T>
    void registerType()
    {
        using namespace Athena::Log;

        // Declarations
        tCreationsInfosNativeIterator iter;

        ATHENA_LOG_EVENT2("Components manager", "Registering a new type of component: '" + T::TYPE + "'");

        // Search if the type is already defined
        iter = m_types.find(T::TYPE);
        if (iter != m_types.end())
        {
            ATHENA_LOG_ERROR2("Components manager", "The type of component '" + T::TYPE + "' was already registered");
            return;
        }

        m_types[T::TYPE] = new TemplatedComponentCreationInfos<T>();
    }


    //_____ Attributes __________
private:
    tCreationsInfosList m_types;    ///< The registered types
};

}
}

#endif
