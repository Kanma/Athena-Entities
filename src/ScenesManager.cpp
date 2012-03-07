/** @file   ScenesManager.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Entities::ScenesManager'
*/

#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Scene.h>
#include <Athena-Core/Log/LogManager.h>


using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Log;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Scenes manager";


/********************************** STATIC ATTRIBUTES ***********************************/

/// The instance of the singleton
template<> ScenesManager* Singleton<ScenesManager>::ms_Singleton = 0;



/****************************** CONSTRUCTION / DESTRUCTION ******************************/

ScenesManager::ScenesManager()
: m_pCurrentScene(0)
{
    ATHENA_LOG_EVENT("Creation");
}

//-----------------------------------------------------------------------

ScenesManager::~ScenesManager()
{
    ATHENA_LOG_EVENT("Destruction");

    destroyAll();
}

//-----------------------------------------------------------------------

ScenesManager& ScenesManager::getSingleton()
{
    assert(ms_Singleton);
    return *ms_Singleton;
}

//-----------------------------------------------------------------------

ScenesManager* ScenesManager::getSingletonPtr()
{
    return ms_Singleton;
}


/******************************** MANAGEMENT OF THE SCENES ******************************/

Scene* ScenesManager::create(const std::string& strName)
{
    // Assertions
    assert(!strName.empty() && "Invalid entity name");

    return new Scene(strName);
}

//-----------------------------------------------------------------------

Scene* ScenesManager::getScene(const std::string& strName)
{
    assert(!strName.empty() && "The name is empty");

    // Declarations
    tScenesNativeIterator iter, iterEnd;

    // Search the entity
    for (iter = m_scenes.begin(), iterEnd = m_scenes.end(); iter != iterEnd; ++iter)
    {
        if ((*iter)->getName() == strName)
        {
            // Return it
            return (*iter);
        }
    }

    // Not found
    return 0;
}

//-----------------------------------------------------------------------

void ScenesManager::destroy(const std::string& strName)
{
    // Assertions
    assert(!strName.empty() && "The name is empty");

    // Declarations
    Scene* pScene;

    // Search the entity
    pScene = getScene(strName);
    if (pScene)
    {
        destroy(pScene);
    }
    else
    {
        ATHENA_LOG_ERROR("Failed to destroy the scene '" + strName + "'");
        assert(false);  // A scene not registered with this manager is not possible
    }
}

//-----------------------------------------------------------------------

void ScenesManager::destroy(Scene* pScene)
{
    // Assertions
    assert(pScene && "Invalid scene");

    delete pScene;
}

//-----------------------------------------------------------------------

void ScenesManager::destroyAll()
{
    while (!m_scenes.empty())
        destroy(m_scenes.front());
}

//-----------------------------------------------------------------------

void ScenesManager::_registerScene(Scene* pScene)
{
    // Assertions
    assert(pScene && "Invalid scene");

    // Add the scene to the list
    m_scenes.push_back(pScene);
}

//-----------------------------------------------------------------------

void ScenesManager::_destroyScene(Scene* pScene)
{
    // Assertions
    assert(pScene && "Invalid scene");

    // Declarations
    tScenesNativeIterator iter, iterEnd;

    // Search the scene
    for (iter = m_scenes.begin(), iterEnd = m_scenes.end(); iter != iterEnd; ++iter)
    {
        if (*iter == pScene)
        {
            // Remove it from the list
            m_scenes.erase(iter);
            break;
        }
    }

    if (m_pCurrentScene == pScene)
        m_pCurrentScene = 0;
}

//-----------------------------------------------------------------------

void ScenesManager::_onSceneShown(Scene* pScene)
{
    // Assertions
    assert(pScene);
    assert(!pScene->isShown());

    if (m_pCurrentScene)
        m_pCurrentScene->hide();

    m_pCurrentScene = pScene;
}

//-----------------------------------------------------------------------

void ScenesManager::_onSceneHidden(Scene* pScene)
{
    // Assertions
    assert(pScene);
    assert(pScene->isShown());

    m_pCurrentScene = 0;
}
