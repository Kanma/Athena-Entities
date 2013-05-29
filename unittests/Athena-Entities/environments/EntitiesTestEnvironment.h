#ifndef _ENTITIESTESTENVIRONMENT_H_
#define _ENTITIESTESTENVIRONMENT_H_

#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Scene.h>
#include <Athena-Core/Data/LocationManager.h>
#include <Athena-Core/Data/DataStream.h>
#include <Athena-Core/Log/LogManager.h>


struct EntitiesTestEnvironment
{
    Athena::Entities::Scene* pScene;
    Athena::Entities::ComponentsManager* pComponentsManager;
    Athena::Entities::ScenesManager* pScenesManager;
    Athena::Data::LocationManager* pLocationManager;
    Athena::Log::LogManager* pLogManager;

    EntitiesTestEnvironment()
    : pScene(0)
    {
        pLogManager = new Athena::Log::LogManager();

        pLocationManager = new Athena::Data::LocationManager();
        pLocationManager->addLocation("unittests", ATHENA_ENTITIES_UNITTESTS_DATA_PATH);

        pScenesManager = new Athena::Entities::ScenesManager();
        pComponentsManager = new Athena::Entities::ComponentsManager();

        pScene = new Athena::Entities::Scene("default");
    }

    ~EntitiesTestEnvironment()
    {
        delete pScene;

        delete pComponentsManager;
        delete pScenesManager;
        delete pLocationManager;
        delete pLogManager;
    }


    std::string readFile(const std::string& strFilename)
    {
        std::string content;

        Athena::Data::DataStream* pStream = pLocationManager->open("unittests", strFilename);
        if (pStream)
        {
            char buffer[1025];

            while (!pStream->eof())
            {
                size_t count = pStream->read(buffer, 1024);
                buffer[count] = 0;
                content += buffer;
            }

            delete pStream;
        }

        return content;
    }
};


#endif
