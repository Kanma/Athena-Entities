/** @file   ScenesManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Entities::ScenesManager'
*/

#ifndef _ATHENA_ENTITIES_SCENESMANAGER_H_
#define _ATHENA_ENTITIES_SCENESMANAGER_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief  Represents the manager of the scenes
///
/// This manager is responsible of the creation/destruction of the scenes.
///
/// There is two ways to create a scene: instanciate a 'Scene' object (on the stack!),
/// which will automatically register itself with the manager, OR ask the manager to create
/// the scene.
///
/// At shutdown, the destruction of the remaining scenes is handled by the manager.
///
/// At any moment, there can only be one shown scene.
///
/// @remark This class is a singleton
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL ScenesManager: public Utils::Singleton<ScenesManager>
{
    //_____ Internal types __________
public:
    typedef std::vector<Scene*>                 tScenesList;
    typedef Utils::VectorIterator<tScenesList>  tScenesIterator;
    typedef tScenesList::iterator               tScenesNativeIterator;


    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    ScenesManager();

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    ~ScenesManager();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a reference to the instance of the scenes manager
    /// @return A reference to the manager
    //------------------------------------------------------------------------------------
    static ScenesManager& getSingleton();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a pointer to the instance of the scenes manager
    /// @return A pointer to the manager
    //------------------------------------------------------------------------------------
    static ScenesManager* getSingletonPtr();


    //_____ Management of the scenes __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Create a new scene
    ///
    /// @param  strName     Name of the scene
    /// @return             The new scene, 0 if failed
    //------------------------------------------------------------------------------------
    Scene* create(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief  Returns a scene
    ///
    /// @param  strName     Name of the scene
    /// @return             The scene, 0 if not found
    //------------------------------------------------------------------------------------
    Scene* getScene(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief  Returns a scene
    ///
    /// @param  uiIndex     Index of the scene
    /// @return             The scene
    //------------------------------------------------------------------------------------
    inline Scene* getScene(unsigned int uiIndex) const
    {
        assert(uiIndex < getNbScenes());
        return m_scenes[uiIndex];
    }

    //------------------------------------------------------------------------------------
    /// @brief  Destroy a scene
    ///
    /// @param  strName     Name of the scene
    //------------------------------------------------------------------------------------
    void destroy(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief  Destroy a scene
    ///
    /// @param  pScene  The scene
    //------------------------------------------------------------------------------------
    void destroy(Scene* pScene);

    //------------------------------------------------------------------------------------
    /// @brief  Destroy all the scenes
    //------------------------------------------------------------------------------------
    void destroyAll();

    //------------------------------------------------------------------------------------
    /// @brief  Returns an iterator over the scenes
    //------------------------------------------------------------------------------------
    inline tScenesIterator getScenesIterator()
    {
        return tScenesIterator(m_scenes.begin(), m_scenes.end());
    }

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of scenes
    //------------------------------------------------------------------------------------
    inline unsigned int getNbScenes() const
    {
        return (unsigned int) m_scenes.size();
    }

    //------------------------------------------------------------------------------------
    /// @brief  Called automatically when a scene is created, to register it into the list
    ///         maintained by the manager
    ///
    /// @param  pScene  The created scene
    //------------------------------------------------------------------------------------
    void _registerScene(Scene* pScene);

    //------------------------------------------------------------------------------------
    /// @brief  Called automatically when an scene is destroyed, to remove it from the list
    ///         maintained by the manager
    ///
    /// @param  pScene  The destroyed scene
    //------------------------------------------------------------------------------------
    void _destroyScene(Scene* pScene);

    //------------------------------------------------------------------------------------
    /// @brief  Called automatically when a scene is shown
    ///
    /// @param  pScene  The scene
    //------------------------------------------------------------------------------------
    void _onSceneShown(Scene* pScene);

    //------------------------------------------------------------------------------------
    /// @brief  Called automatically when a scene is hidden
    ///
    /// @param  pScene  The scene
    //------------------------------------------------------------------------------------
    void _onSceneHidden(Scene* pScene);

    //------------------------------------------------------------------------------------
    /// @brief  Returns the scenes currently shown
    //------------------------------------------------------------------------------------
    inline Scene* getCurrentScene()
    {
        return m_pCurrentScene;
    }


    //_____ Attributes __________
private:
    tScenesList m_scenes;           ///< The scenes
    Scene*      m_pCurrentScene;    ///< The scene currently shown
};

}
}

#endif
