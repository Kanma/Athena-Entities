#ifndef _ENTITIESTESTENVIRONMENT_H_
#define _ENTITIESTESTENVIRONMENT_H_

#include <Athena-Entities/ComponentsManager.h>
//#include <Athena-Core/Log/LogManager.h>


struct EntitiesTestEnvironment
{
//	Athena::Entities::CScene* pScene;
    Athena::Entities::ComponentsManager* pComponentsManager;

	EntitiesTestEnvironment()
//	: pScene(0)
	{
        // new Athena::Log::CLogManager();
        // new Athena::Entities::CScenesManager();
        pComponentsManager = new Athena::Entities::ComponentsManager();
         
        // pScene = new Athena::Entities::CScene("default");
	}

	~EntitiesTestEnvironment()
	{
        // delete pScene;
        // 
        delete pComponentsManager;
        // delete Athena::pScenesManager;
        // delete Athena::pLogManager;
	}
};


#endif
