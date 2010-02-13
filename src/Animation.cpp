/**	@file	Animation.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Entities::Animation'
*/

#include <Athena-Entities/Animation.h>
#include <Athena-Entities/ComponentAnimation.h>


using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace std;


/***************************** CONSTRUCTION / DESTRUCTION *******************************/

Animation::Animation(const std::string& strName)
: m_strName(strName), m_fWeight(1.0f), m_fTimePos(0.0f), m_fLength(0.0f), m_bEnabled(false),
  m_bLooping(false)
{
	assert(!strName.empty());
}

//-----------------------------------------------------------------------

Animation::~Animation()
{
}


/************************************** METHODS *****************************************/

void Animation::reset()
{
	tComponentAnimationsIterator iter = getComponentAnimationsIterator();

	while (iter.hasMoreElements())
	{
        ComponentAnimation* pComponentAnimation = iter.getNext();
        pComponentAnimation->setCurrentWeight(pComponentAnimation->getWeight());
        pComponentAnimation->setTimePosition(0.0f);
    }
    
	m_fTimePos = 0.0f;
    m_fWeight = 1.0f;
}

//-----------------------------------------------------------------------

void Animation::update(float fSecondsElapsed)
{
	setTimePosition(m_fTimePos + fSecondsElapsed);
}

//-----------------------------------------------------------------------

void Animation::setEnabled(bool bEnabled)
{
	tComponentAnimationsIterator iter = getComponentAnimationsIterator();

	while (iter.hasMoreElements())
		iter.getNext()->setEnabled(bEnabled);
		
    m_bEnabled = bEnabled;
}

//-----------------------------------------------------------------------

void Animation::setTimePosition(float fTimePos)
{
	if (fTimePos != m_fTimePos)
	{
		m_fTimePos = fTimePos;
		
		if (m_bLooping)
		{
			// Wrap
			m_fTimePos = fmod(m_fTimePos, m_fLength);
			if (m_fTimePos < 0.0f)
				m_fTimePos += m_fLength;     
		}
		else
		{
			// Clamp
			if (m_fTimePos < 0.0f)
				m_fTimePos = 0.0f;
			else if (m_fTimePos > m_fLength)
				m_fTimePos = m_fLength;
		}

    	tComponentAnimationsIterator iter = getComponentAnimationsIterator();

		while (iter.hasMoreElements())
    	{
            ComponentAnimation* pComponentAnimation = iter.getNext();
			
			if (fTimePos >= pComponentAnimation->getOffset() + pComponentAnimation->getLength())
			    pComponentAnimation->setTimePosition(pComponentAnimation->getLength());
			else if (fTimePos >= pComponentAnimation->getOffset())
			    pComponentAnimation->setTimePosition(fTimePos - pComponentAnimation->getOffset());
			else
			    pComponentAnimation->setTimePosition(0.0f);
		}
	}
}

//-----------------------------------------------------------------------

void Animation::setWeight(float fWeight)
{
	m_fWeight = fWeight;

	tComponentAnimationsIterator iter = getComponentAnimationsIterator();

	while (iter.hasMoreElements())
	{
        ComponentAnimation* pComponentAnimation = iter.getNext();
		pComponentAnimation->setCurrentWeight(pComponentAnimation->getWeight() * m_fWeight);
	}
}

//-----------------------------------------------------------------------

void Animation::setLooping(bool bLoop)
{
	tComponentAnimationsIterator iter = getComponentAnimationsIterator();

	while (iter.hasMoreElements())
		iter.getNext()->setLooping(bLoop);
		
    m_bLooping = bLoop;
}


/********************** MANAGEMENT OF THE COMPONENTS ANIMATIONS *************************/

void Animation::addComponentAnimation(ComponentAnimation* pComponentAnimation)
{
    // Assertions
    assert(pComponentAnimation);

	m_componentAnimations.push_back(pComponentAnimation);

	if (pComponentAnimation->getLength() + pComponentAnimation->getOffset() > m_fLength)
		m_fLength = pComponentAnimation->getLength() + pComponentAnimation->getOffset();
		
    pComponentAnimation->setEnabled(m_bEnabled);
    pComponentAnimation->setLooping(m_bLooping);

	pComponentAnimation->setCurrentWeight(pComponentAnimation->getWeight() * m_fWeight);
}

//-----------------------------------------------------------------------

void Animation::removeComponentAnimation(ComponentAnimation* pComponentAnimation)
{
	tComponentAnimationsList::iterator iter, iterEnd;
	for (iter = m_componentAnimations.begin(), iterEnd = m_componentAnimations.end();
	     iter != iterEnd; ++iter)
	{
		if ((*iter) == pComponentAnimation)
		{
			m_componentAnimations.erase(iter);
			break;
		}
	}

	m_fLength = 0.0f;

	tComponentAnimationsIterator iter2 = getComponentAnimationsIterator();
	while (iter2.hasMoreElements())
	{
		ComponentAnimation* pComponentAnimation = iter2.getNext();

		if (pComponentAnimation->getLength() + pComponentAnimation->getOffset() > m_fLength)
			m_fLength = pComponentAnimation->getLength() + pComponentAnimation->getOffset();
	}
}
