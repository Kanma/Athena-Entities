#ifndef _ENTITIESTESTENVIRONMENT_H_
#define _ENTITIESTESTENVIRONMENT_H_

//#include <Athena-Entities/ComponentsManager.h>
//#include <Athena-Core/Log/LogManager.h>


struct EntitiesTestEnvironment
{
//	Athena::Entities::CScene* pScene;

	EntitiesTestEnvironment()
//	: pScene(0)
	{
        // new Athena::Log::CLogManager();
        // new Athena::Entities::CScenesManager();
        // new Athena::Entities::CComponentsManager();
        // 
        // pScene = new Athena::Entities::CScene("default");
	}

	~EntitiesTestEnvironment()
	{
        // delete pScene;
        // 
        // delete Athena::pComponentsManager;
        // delete Athena::pScenesManager;
        // delete Athena::pLogManager;
	}
};


#endif
