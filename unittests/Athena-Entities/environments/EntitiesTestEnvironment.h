#ifndef _ENTITIESTESTENVIRONMENT_H_
#define _ENTITIESTESTENVIRONMENT_H_

#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Scene.h>


struct EntitiesTestEnvironment
{
    Athena::Entities::Scene* pScene;
    Athena::Entities::ComponentsManager* pComponentsManager;
    Athena::Entities::ScenesManager* pScenesManager;

    EntitiesTestEnvironment()
    : pScene(0)
    {
        pScenesManager = new Athena::Entities::ScenesManager();
        pComponentsManager = new Athena::Entities::ComponentsManager();

        pScene = new Athena::Entities::Scene("default");
    }

    ~EntitiesTestEnvironment()
    {
        delete pScene;

        delete pComponentsManager;
        delete pScenesManager;
    }
};


#endif
