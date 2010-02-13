/**	@file	AnimationsMixer.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::AnimationsMixer'
*/

#include <Athena-Entities/AnimationsMixer.h>
#include <Athena-Entities/Animation.h>


using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace std;


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

AnimationsMixer::AnimationsMixer()
: m_pCurrentAnimation(0), m_pPreviousAnimation(0)
{
}

//-----------------------------------------------------------------------

AnimationsMixer::~AnimationsMixer()
{
	tAnimationsIterator iter = getAnimationsIterator();

	while (iter.hasMoreElements())
		delete iter.getNext();

	m_animations.clear();
}


/************************************** METHODS *****************************************/

void AnimationsMixer::update(float fSecondsElapsed)
{
	// If the previous animation isn't finished yet (because it's blended with
	// the current one), update it
	if (m_pPreviousAnimation)
	{
		// Compute the new weight of the previous animation
		float fWeight = m_pPreviousAnimation->getWeight();
		fWeight -= fSecondsElapsed * 10.0f;
		
		// If the weight goes to zero, disable the previous animation 
		if (fWeight <= 0.0f)
		{
			m_pPreviousAnimation->setEnabled(false);
			m_pCurrentAnimation->setWeight(1.0f);
			m_pPreviousAnimation = 0;
		}
		// Otherwise, continue to blend the 2 animations with the new weight
		else
		{
			m_pPreviousAnimation->setWeight(fWeight);
			m_pCurrentAnimation->setWeight(1.0f - fWeight);
		}
	}

	// Add the elapsed time to the animations
	if (m_pCurrentAnimation)
		m_pCurrentAnimation->update(fSecondsElapsed);

	if (m_pPreviousAnimation)
		m_pPreviousAnimation->update(fSecondsElapsed);
}


/*************************** MANAGEMENT OF THE ANIMATIONS *******************************/

bool AnimationsMixer::addAnimation(tAnimation animation, Animation* pAnimation)
{
	// Assertions
	assert(pAnimation);
	assert(animation > 0);

	// Check that the animation ID doesn't already exists in the list
	tAnimationsList::iterator iter = m_animations.find(animation);
	if (iter != m_animations.end())
		return false;

	// Add the animation in the list
	m_animations[animation] = pAnimation;

	return true;
}

//-----------------------------------------------------------------------

tAnimation AnimationsMixer::addAnimation(Animation* pAnimation)
{
	// Assertions
	assert(pAnimation);

	// Create an unique ID for the animation
	tAnimation animation = m_animations.rbegin()->first + 1;

	// Add the animation in the list
	m_animations[animation] = pAnimation;

	return animation;
}

//-----------------------------------------------------------------------

void AnimationsMixer::removeAnimation(tAnimation animation)
{
	assert(animation > 0);

	// Check that the animation ID exists in the list
	tAnimationsList::iterator iter = m_animations.find(animation);
	if (iter != m_animations.end())
	{
		delete iter->second;
		m_animations.erase(iter);
	}
}


void AnimationsMixer::removeAnimation(const std::string& strName)
{
	assert(!strName.empty());

	tAnimationsList::iterator iter, iterEnd;
	
	for (iter = m_animations.begin(), iterEnd = m_animations.end(); iter != iterEnd; ++iter)
	{
		if (iter->second->getName() == strName)
		{
			delete iter->second;
			m_animations.erase(iter);
			break;
		}
	}
}

//-----------------------------------------------------------------------

void AnimationsMixer::removeAnimation(Animation* pAnimation)
{
	assert(pAnimation);

	tAnimationsList::iterator iter, iterEnd;
	
	for (iter = m_animations.begin(), iterEnd = m_animations.end(); iter != iterEnd; ++iter)
	{
		if (iter->second == pAnimation)
		{
			delete iter->second;
			m_animations.erase(iter);
			break;
		}
	}
}

//-----------------------------------------------------------------------

Animation* AnimationsMixer::getAnimation(tAnimation animation)
{
	assert(animation > 0);

	tAnimationsList::iterator iter = m_animations.find(animation);
	if (iter != m_animations.end())
		return iter->second;

	return 0;
}

//-----------------------------------------------------------------------

Animation* AnimationsMixer::getAnimation(const std::string& strName)
{
	tAnimationsIterator iter = getAnimationsIterator();

	while (iter.hasMoreElements())
	{
		if (iter.peekNextValue()->getName() == strName)
			return iter.peekNextValue();

		iter.moveNext();
	}

	return 0;
}

//-----------------------------------------------------------------------

void AnimationsMixer::startAnimation(tAnimation animation, bool bReset)
{
	Animation* pNewAnimation = getAnimation(animation);
	if (pNewAnimation)
		startAnimation(pNewAnimation, bReset);
}

//-----------------------------------------------------------------------

void AnimationsMixer::startAnimation(const std::string& strName, bool bReset)
{
	Animation* pNewAnimation = getAnimation(strName);
	if (pNewAnimation)
		startAnimation(pNewAnimation, bReset);
}

//-----------------------------------------------------------------------

void AnimationsMixer::startAnimation(Animation* pNewAnimation, bool bReset)
{
	assert(pNewAnimation);

	if (bReset)
		stopAnimation();

	// Test if there is currently no animation played
	if (!m_pCurrentAnimation)
	{
		m_pCurrentAnimation = pNewAnimation;
		m_pCurrentAnimation->setTimePosition(0.0f);
		m_pCurrentAnimation->setWeight(1.0f);
		m_pCurrentAnimation->setEnabled(true);
	}

	// Test if the animation currently played is the same than the new one
	else if (m_pCurrentAnimation == pNewAnimation)
	{
		if (m_pPreviousAnimation)
		{
			m_pPreviousAnimation->setEnabled(false);
			m_pPreviousAnimation = 0;
		}

		m_pCurrentAnimation->setTimePosition(0.0f);
		m_pCurrentAnimation->setWeight(1.0f);
		m_pCurrentAnimation->setEnabled(true);
	}

	// We must blend the new animation with the previous one
	else
	{
		if (m_pPreviousAnimation)
		{
			m_pPreviousAnimation->setEnabled(false);
			m_pPreviousAnimation = 0;
		}

		m_pPreviousAnimation = m_pCurrentAnimation;
		m_pCurrentAnimation = pNewAnimation;

		m_pPreviousAnimation->setWeight(1.0f);
		m_pCurrentAnimation->setTimePosition(0.0f);
		m_pCurrentAnimation->setWeight(0.0f);
		m_pCurrentAnimation->setEnabled(true);
	}
}

//-----------------------------------------------------------------------

void AnimationsMixer::stopAnimation()
{
	if (m_pPreviousAnimation)
	{
		m_pPreviousAnimation->setEnabled(false);
		m_pPreviousAnimation = 0;
	}

	if (m_pCurrentAnimation)
	{
		m_pCurrentAnimation->setEnabled(false);
		m_pCurrentAnimation = 0;
	}
}

//-----------------------------------------------------------------------

bool AnimationsMixer::isCurrentAnimationDone()
{
	if (m_pCurrentAnimation && !m_pCurrentAnimation->isLooping())
		return (m_pCurrentAnimation->getTimePosition() >= m_pCurrentAnimation->getLength());

	return false;
}

//-----------------------------------------------------------------------

float AnimationsMixer::getCurrentAnimationTime()
{
	if (m_pCurrentAnimation)
		return m_pCurrentAnimation->getTimePosition();

	return 0.0f;
}

//-----------------------------------------------------------------------

float AnimationsMixer::getCurrentAnimationLength()
{
	if (m_pCurrentAnimation)
		return m_pCurrentAnimation->getTimePosition();

	return 0.0f;
}

//-----------------------------------------------------------------------

tAnimation AnimationsMixer::getCurrentAnimationID()
{
	tAnimationsIterator iter = getAnimationsIterator();

	while (iter.hasMoreElements())
	{
		if (iter.peekNextValue() == m_pCurrentAnimation)
			return iter.peekNextKey();

		iter.moveNext();
	}

	return 0;
}
