/** @file	Animation.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Entities::Animation'
*/

#ifndef _ATHENA_ENTITIES_ANIMATION_H_
#define _ATHENA_ENTITIES_ANIMATION_H_

#include <Athena-Entities/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Entities {

//----------------------------------------------------------------------------------------
/// @brief	Represents an animation
///
/// An animation is a collection of 'components animations' (see ComponentAnimation)
/// that are all played at the same time. A weight can by assigned to each component
/// animation.
///
/// The length of the animation is the higher length of its components animations (taking
/// their offsets into account). Those offsets are used to make one component animation
/// starts after the others.
//----------------------------------------------------------------------------------------
class ATHENA_ENTITIES_SYMBOL Animation
{
	//_____ Internal types __________
public:
	typedef std::vector<ComponentAnimation*>		    	tComponentAnimationsList;
	typedef Utils::VectorIterator<tComponentAnimationsList>	tComponentAnimationsIterator;


	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    //------------------------------------------------------------------------------------
	Animation(const std::string& strName);
	
    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~Animation();


	//_____ Methods __________
public:
	//------------------------------------------------------------------------------------
	/// @brief	Returns the name of the animation
	//------------------------------------------------------------------------------------
	inline const std::string& getName() const
	{
	    return m_strName;
	}

	//------------------------------------------------------------------------------------
	/// @brief	Reset the animation (go at time 0)
	//------------------------------------------------------------------------------------
	void reset();

    //------------------------------------------------------------------------------------
    /// @brief	Update the animation
    ///
    /// @param  fSecondsElapsed     The number of seconds elapsed since the last update
    //------------------------------------------------------------------------------------
	void update(float fSecondsElapsed);

	//------------------------------------------------------------------------------------
	/// @brief	Enable/Disable the animation
	//------------------------------------------------------------------------------------
	void setEnabled(bool bEnabled);

	//------------------------------------------------------------------------------------
	/// @brief	Indicates if the animation is enabled
	//------------------------------------------------------------------------------------
	inline bool isEnabled() const
	{
		return m_bEnabled;
	}

	//------------------------------------------------------------------------------------
	/// @brief	Change the current time position of the animation
	///
	/// @param  fTimePos    New time position in seconds
	//------------------------------------------------------------------------------------
	void setTimePosition(float fTimePos);

	//------------------------------------------------------------------------------------
	/// @brief	Returns the current time position of the animation, in seconds
	//------------------------------------------------------------------------------------
	inline float getTimePosition() const
	{
		return m_fTimePos;
	}

	//------------------------------------------------------------------------------------
	/// @brief	Change the weight of the animation
	///
	/// @param  fWeigth    New weight
	///
	/// @remark Used by the animations mixer during a transition between two animations
	//------------------------------------------------------------------------------------
	void setWeight(float fWeight);
	
	//------------------------------------------------------------------------------------
	/// @brief	Returns the weight of the animation
	//------------------------------------------------------------------------------------
	inline float getWeight() const
	{
		return m_fWeight;
	}

	//------------------------------------------------------------------------------------
	/// @brief	Returns the length of the animation, in seconds
	//------------------------------------------------------------------------------------
	inline float getLength() const
	{
		return m_fLength;
	}

	//------------------------------------------------------------------------------------
	/// @brief	Enable/Disable the looping of the animation
	//------------------------------------------------------------------------------------
    void setLooping(bool bLoop);

	//------------------------------------------------------------------------------------
	/// @brief	Indicates if the loopîng of the animation is enabled
	//------------------------------------------------------------------------------------
	inline bool isLooping() const
	{
		return m_bLooping;
	}


	//_____ Management of the components animations __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns the number of component animations in the list
    //------------------------------------------------------------------------------------
	inline unsigned int getNbComponentAnimations() const
	{
	    return (unsigned int) m_componentAnimations.size();
	}

    //------------------------------------------------------------------------------------
    /// @brief	Adds a component animation to the list
    ///
    /// @param  pComponentAnimation     The component animation
    ///
    /// @remark The offset and length of the component animation must be set BEFORE this
    ///         method is called, and not modified later!
    //------------------------------------------------------------------------------------
	void addComponentAnimation(ComponentAnimation* pComponentAnimation);

    //------------------------------------------------------------------------------------
    /// @brief	Remove a component animation from the list
    ///
    /// @param  pComponentAnimation     The component animation
    //------------------------------------------------------------------------------------
	void removeComponentAnimation(ComponentAnimation* pComponentAnimation);

    //------------------------------------------------------------------------------------
    /// @brief	Returns an iterator over the component animations
    //------------------------------------------------------------------------------------
	inline tComponentAnimationsIterator getComponentAnimationsIterator()
	{
		return tComponentAnimationsIterator(m_componentAnimations.begin(), m_componentAnimations.end());
	}


	//_____ Attributes __________
private:
	std::string		            m_strName;			    ///< Animation's name
	tComponentAnimationsList	m_componentAnimations;	///< The list of component animations
	float				        m_fWeight;			    ///< Current weigth
	float				        m_fTimePos;             ///< Current time position
	float				        m_fLength;              ///< Length
	bool				        m_bEnabled;             ///< Indicates if the animation is enabled
	bool				        m_bLooping;             ///< Indicates if the looping is enabled
};

}
}

#endif
