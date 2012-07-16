/** @file   Scripting.h
    @author Philip Abbet

    Scripting-related declarations for the Athena-Entities module
*/

#ifndef _ATHENA_ENTITIES_SCRIPTING_H_
#define _ATHENA_ENTITIES_SCRIPTING_H_

#include <v8.h>
#include <Athena-Entities/tComponentID.h>


namespace Athena {
namespace Entities {

    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Animation wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Animation* fromJSAnimation(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Animation (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSAnimation();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Animation around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(Animation* pAnimation);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ AnimationsMixer wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL AnimationsMixer* fromJSAnimationsMixer(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript AnimationsMixer (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSAnimationsMixer();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript AnimationsMixer around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(AnimationsMixer* pMixer);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ ComponentAnimation wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL ComponentAnimation* fromJSComponentAnimation(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript ComponentAnimation (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSComponentAnimation();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript ComponentAnimation around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(ComponentAnimation* pComponentAnimation);


    //------------------------------------------------------------------------------------
    /// @brief  Create a C++ ComponentID from a JavaScript one
    //------------------------------------------------------------------------------------
    tComponentID ATHENA_ENTITIES_SYMBOL fromJSComponentID(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a JavaScript ComponentID from a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(const tComponentID& value);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ ComponentsList wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL ComponentsList* fromJSComponentsList(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript ComponentsList (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSComponentsList();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript ComponentsList around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(ComponentsList* pList);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Component wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Component* fromJSComponent(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Component (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSComponent();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Component around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(Component* pComponent);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Entity wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Entity* fromJSEntity(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Entity (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSEntity();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Entity around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(Entity* pEntity);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Scene wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Scene* fromJSScene(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Scene (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSScene();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Scene around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(Scene* pScene);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Transforms wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_ENTITIES_SYMBOL Transforms* fromJSTransforms(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Transforms (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_ENTITIES_SYMBOL createJSTransforms();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Transforms around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_ENTITIES_SYMBOL toJavaScript(Transforms* pTransforms);
}
}

#endif
