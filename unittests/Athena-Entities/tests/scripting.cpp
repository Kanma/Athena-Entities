#include <UnitTest++.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Entities/Scripting.h>
#include "../environments/ScriptingTestEnvironment.h"
#include <iostream>


using namespace Athena::Scripting;
using namespace Athena::Entities;
using namespace v8;


#define JS_TEST(NAME)                                                \
    TEST_FIXTURE(ScriptingTestEnvironment, NAME)                     \
    {                                                                \
        HandleScope handle_scope;                                    \
                                                                     \
        Handle<Value> result = pScriptingManager->executeFile(       \
                ATHENA_ENTITIES_UNITTESTS_SCRIPTS_PATH #NAME ".js"); \
                                                                     \
        CHECK(!result.IsEmpty());                                    \
        CHECK(pScriptingManager->getLastError().empty());            \
    }


SUITE(Conversions)
{
    TEST_FIXTURE(ScriptingTestEnvironment, ConvertFullComponentIDFromJavaScript)
    {
        HandleScope handle_scope;

        Handle<Value> result = pScriptingManager->execute("new Athena.Entities.ComponentID('Transforms://myentity:mycomponent');");

        tComponentID id = fromJSComponentID(result);

        CHECK_EQUAL(COMP_TRANSFORMS, id.type);
        CHECK_EQUAL("myentity", id.strEntity);
        CHECK_EQUAL("mycomponent", id.strName);
    }


    TEST_FIXTURE(ScriptingTestEnvironment, ConvertPartialComponentIDFromJavaScript)
    {
        HandleScope handle_scope;

        Handle<Value> result = pScriptingManager->execute("new Athena.Entities.ComponentID('Transforms://mycomponent');");

        tComponentID id = fromJSComponentID(result);

        CHECK_EQUAL(COMP_TRANSFORMS, id.type);
        CHECK_EQUAL("", id.strEntity);
        CHECK_EQUAL("mycomponent", id.strName);
    }
}


SUITE(ComponentsList)
{
    JS_TEST(ComponentsList_Creation);
    JS_TEST(ComponentsList_AddComponent);
}
