/** @file   AnimationsMixer.h
    @author Philip Abbet

    Declaration of the class 'Athena::Entities::AnimationsMixer'
*/

#ifndef _ATHENA_ENTITIES_ANIMATIONSMIXER_H_
#define _ATHENA_ENTITIES_ANIMATIONSMIXER_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {

//----------------------------------------------------------------------------------------
/// @brief  Maintains a list of the animations of an entity, and is able to smoothly
///         go from one animation to another one
///
/// The animation currently played is called the current one.
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL AnimationsMixer
{
    //_____ Internal types __________
public:
    typedef std::map<tAnimation, Animation*>    tAnimationsList;
    typedef Utils::MapIterator<tAnimationsList> tAnimationsIterator;


    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    AnimationsMixer();

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    ~AnimationsMixer();


    //_____ Management of the current animation __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Update the current animation
    ///
    /// @param  fSecondsElapsed     The number of seconds elapsed since the last update
    //------------------------------------------------------------------------------------
    void update(float fSecondsElapsed);

    //------------------------------------------------------------------------------------
    /// @brief  Starts to play an animation
    ///
    /// The animation becomes the current one
    /// @param  animation   ID of the animation
    /// @param  bReset      'true' to disable the transition from the current animation to
    ///                     the new one
    //------------------------------------------------------------------------------------
    void startAnimation(tAnimation animation, bool bReset = false);

    //------------------------------------------------------------------------------------
    /// @brief  Starts to play an animation
    ///
    /// The animation becomes the current one
    /// @param  strName     Name of the animation
    /// @param  bReset      'true' to disable the transition from the current animation to
    ///                     the new one
    //------------------------------------------------------------------------------------
    void startAnimation(const std::string& strName, bool bReset = false);

    //------------------------------------------------------------------------------------
    /// @brief  Stop the current animation
    ///
    /// There will be no 'current animation' after that
    //------------------------------------------------------------------------------------
    void stopAnimation();

    //------------------------------------------------------------------------------------
    /// @brief  Indicates if the current animation is done playing
    ///
    /// Only works for non-looping animations
    //------------------------------------------------------------------------------------
    bool isCurrentAnimationDone();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current position (in seconds) of the current animation
    //------------------------------------------------------------------------------------
    float getCurrentAnimationTime();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the length (in seconds) of the current animation
    //------------------------------------------------------------------------------------
    float getCurrentAnimationLength();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the ID of the current animation
    //------------------------------------------------------------------------------------
    tAnimation getCurrentAnimationID();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current animation
    //------------------------------------------------------------------------------------
    inline Animation* getCurrentAnimation()
    {
        return m_pCurrentAnimation;
    }


private:
    //------------------------------------------------------------------------------------
    /// @brief  Starts to play an animation
    ///
    /// The animation becomes the current one
    /// @param  pNewAnimation   The animation
    /// @param  bReset          'true' to disable the transition from the current animation
    ///                         to the new one
    //------------------------------------------------------------------------------------
    void startAnimation(Animation* pNewAnimation, bool bReset);


    //_____ Management of the list of animations __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of animations in the list
    //------------------------------------------------------------------------------------
    inline unsigned int getNbAnimations() const
    {
        return (unsigned int) m_animations.size();
    }

    //------------------------------------------------------------------------------------
    /// @brief  Adds an animation to the list
    ///
    /// @param  animation   ID to assign to the animation
    /// @param  pAnimation  The animation
    /// @return             'true' if successful
    //------------------------------------------------------------------------------------
    bool addAnimation(tAnimation animation, Animation* pAnimation);

    //------------------------------------------------------------------------------------
    /// @brief  Adds an animation to the list
    ///
    /// @param  pAnimation  The animation
    /// @return             The ID that was assigned to the animation
    //------------------------------------------------------------------------------------
    tAnimation addAnimation(Animation* pAnimation);

    //------------------------------------------------------------------------------------
    /// @brief  Remove an animation from the list
    ///
    /// @param  animation   ID of the animation
    //------------------------------------------------------------------------------------
    void removeAnimation(tAnimation animation);

    //------------------------------------------------------------------------------------
    /// @brief  Remove an animation from the list
    ///
    /// @param  strName     Name of the animation
    //------------------------------------------------------------------------------------
    void removeAnimation(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief  Remove an animation from the list
    ///
    /// @param  pAnimation  The animation
    //------------------------------------------------------------------------------------
    void removeAnimation(Animation* pAnimation);

    //------------------------------------------------------------------------------------
    /// @brief  Returns an animation from the list
    ///
    /// @param  animation   ID of the animation
    //------------------------------------------------------------------------------------
    Animation* getAnimation(tAnimation animation);

    //------------------------------------------------------------------------------------
    /// @brief  Returns an animation from the list
    ///
    /// @param  strName     Name of the animation
    //------------------------------------------------------------------------------------
    Animation* getAnimation(const std::string& strName);

    //------------------------------------------------------------------------------------
    /// @brief  Returns an iterator over the animations
    //------------------------------------------------------------------------------------
    inline tAnimationsIterator getAnimationsIterator()
    {
        return tAnimationsIterator(m_animations.begin(), m_animations.end());
    }


    //_____ Attributes __________
private:
    tAnimationsList m_animations;           ///< A list of animations
    Animation*      m_pCurrentAnimation;    ///< The animation currently played
    Animation*      m_pPreviousAnimation;   ///< The animation previously played
};

}
}

#endif
