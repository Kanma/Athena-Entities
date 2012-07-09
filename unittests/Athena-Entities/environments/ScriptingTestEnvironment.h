#ifndef _SCRIPTINGTESTENVIRONMENT_H_
#define _SCRIPTINGTESTENVIRONMENT_H_

#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Entities/ComponentsManager.h>
#include <Athena-Entities/ScenesManager.h>
#include <Athena-Entities/Scene.h>
#include <v8.h>


struct ScriptingTestEnvironment
{
    Athena::Entities::Scene* pScene;
    Athena::Entities::ComponentsManager* pComponentsManager;
    Athena::Entities::ScenesManager* pScenesManager;
    Athena::Scripting::ScriptingManager* pScriptingManager;

    ScriptingTestEnvironment()
    : pScene(0)
    {
        v8::HandleScope handle_scope;

        pScenesManager = new Athena::Entities::ScenesManager();
        pComponentsManager = new Athena::Entities::ComponentsManager();

        pScene = new Athena::Entities::Scene("default");

        pScriptingManager = new Athena::Scripting::ScriptingManager();

        pScriptingManager->import("Athena.Entities");

        pScriptingManager->executeFile(ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH "tests.js");
    }

    ~ScriptingTestEnvironment()
    {
        delete pScene;

        delete pScenesManager;
        delete pComponentsManager;

        delete pScriptingManager;
    }
};


#endif
