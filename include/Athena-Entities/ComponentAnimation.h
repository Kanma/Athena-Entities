/** @file   ComponentAnimation.h
    @author Philip Abbet

    Declarations of the class 'Athena::Entities::ComponentAnimation'
*/

#ifndef _ATHENA_ENTITIES_COMPONENTANIMATION_H_
#define _ATHENA_ENTITIES_COMPONENTANIMATION_H_

#include <Athena-Entities/Prerequisites.h>


namespace Athena {
namespace Entities {


//----------------------------------------------------------------------------------------
/// @brief  Base class for the component animations
///
/// Each component animation is defined by:
///   - its length
///   - its time position
///   - its weight (allowing an animation to combine it with another component animation)
///   - its offset (allowing an animation to synchronize the playing of several component
///     animations)
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL ComponentAnimation
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    ComponentAnimation()
    : m_fOffset(0.0f), m_fWeight(1.0f)
    {
    }

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    virtual ~ComponentAnimation()
    {
    }


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Update the animation
    ///
    /// @param  fSecondsElapsed     The number of seconds elapsed since the last update
    //------------------------------------------------------------------------------------
    inline void update(float fElapsedSeconds)
    {
        setTimePosition(getTimePosition() + fElapsedSeconds);
    }

    //------------------------------------------------------------------------------------
    /// @brief  Sets the start time offset
    ///
    /// @param  fOffset     The offset, in seconds
    //------------------------------------------------------------------------------------
    inline void setOffset(float fOffset)
    {
        m_fOffset = fOffset;
    }

    //------------------------------------------------------------------------------------
    /// @brief  Return the start time offset, in seconds
    //------------------------------------------------------------------------------------
    inline float getOffset() const
    {
        return m_fOffset;
    }

    //------------------------------------------------------------------------------------
    /// @brief  Sets the weight (influence)
    /// @param  fWeight     The weight
    //------------------------------------------------------------------------------------
    inline void setWeight(float fWeight)
    {
        m_fWeight = fWeight;
    }

    //------------------------------------------------------------------------------------
    /// @brief  Returns the weight (influence)
    //------------------------------------------------------------------------------------
    inline float getWeight() const
    {
        return m_fWeight;
    }


    //_____ Methods to implement __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Sets the current time position of this component animation
    ///
    /// @remark Don't use the 'm_fOffset' attribute in your implementation, it was already
    ///         taken into account in the 'fTimePos' parameter
    //------------------------------------------------------------------------------------
    virtual void setTimePosition(float fTimePos) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current time position of this component animation
    ///
    /// @remark Don't use the 'm_fOffset' attribute in your implementation
    //------------------------------------------------------------------------------------
    virtual float getTimePosition() const = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Returns the length of this component animation
    ///
    /// @remark Don't use the 'm_fOffset' attribute in your implementation
    //------------------------------------------------------------------------------------
    virtual float getLength() const = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Sets the current weight (influence) of this component animation
    ///
    /// @remark Don't use the 'm_fWeight' attribute in your implementation, it was already
    ///         taken into account in the 'fWeight' parameter
    //------------------------------------------------------------------------------------
    virtual void setCurrentWeight(float fWeight) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current weight (influence) of this component animation
    ///
    /// @remark Don't use the 'm_fWeight' attribute in your implementation
    //------------------------------------------------------------------------------------
    virtual float getCurrentWeight() const = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Enables/Disables the component animation
    //------------------------------------------------------------------------------------
    virtual void setEnabled(bool bEnabled) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Sets whether or not the component animation must loops at the start and
    ///         end of the animation if the time continues to be altered
    //------------------------------------------------------------------------------------
    virtual void setLooping(bool bLoop) = 0;


    //_____ Attributes __________
private:
    float   m_fOffset;  ///< Start time offset
    float   m_fWeight;  ///< Weight of the component animation
};

}
}

#endif
