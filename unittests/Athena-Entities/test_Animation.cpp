#include <UnitTest++.h>
#include <Athena-Entities/Animation.h>
#include <Athena-Entities/ComponentAnimation.h>
#include <Athena-Core/Utils/Iterators.h>


using namespace Athena::Entities;
using namespace Athena::Utils;


class MinMaxAnimation: public ComponentAnimation
{
public:
    MinMaxAnimation(float min = 0.0f, float max = 10.0f, float length = 10.0f)
    : min(min), max(max), length(length), timepos(0.0f), weight(1.0f),
      loop(false), enabled(false)
    {
    }

    virtual ~MinMaxAnimation()
    {
    }

    virtual void setTimePosition(float fTimePos)
    {
        if (!enabled)
            return;

        timepos = fTimePos;
            
        value = min + (max - min) * fTimePos / length;
    }

    virtual float getTimePosition() const
    {
        return timepos;
    }
	
    virtual float getLength() const
    {
        return length;
    }

    virtual void setCurrentWeight(float fWeight)
    {
        weight = fWeight;
    }

    virtual float getCurrentWeight() const
    {
        return weight;
    }

    virtual void setEnabled(bool bEnabled)
    {
        enabled = bEnabled;
    }

    virtual void setLooping(bool bLoop)
    {
        loop = bLoop;
    }


    float min;
    float max;
    float length;
    float timepos;
    float weight;
    float value;
    bool loop;
    bool enabled;
};


class Animable
{
public:
    float value;
    std::vector<MinMaxAnimation*> animations;
    
    void update()
    {
        value = 0.0f;
        
        VectorIterator<std::vector<MinMaxAnimation*> > iter(animations);
        while (iter.hasMoreElements())
        {
            MinMaxAnimation* pAnim = iter.getNext();
            
            value += pAnim->getCurrentWeight() * pAnim->value;
        }
    }    
};



SUITE(AnimationTests)
{
    TEST(Creation)
    {
        Animation anim("test");

        CHECK_EQUAL(0, anim.getNbComponentAnimations());
    }

    TEST(AddOneComponentAnimation)
    {
        MinMaxAnimation a1;
        Animation anim("test");

        anim.addComponentAnimation(&a1);

        CHECK_EQUAL(1, anim.getNbComponentAnimations());
    }

    TEST(AddTwoComponentAnimations)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        CHECK_EQUAL(2, anim.getNbComponentAnimations());
    }

    TEST(AddRemoveComponentAnimation)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        anim.removeComponentAnimation(&a1);

        CHECK_EQUAL(1, anim.getNbComponentAnimations());
    }

    TEST(Enabling)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        anim.setEnabled(false);
        anim.setEnabled(true);

        CHECK(anim.isEnabled());
        CHECK(a1.enabled);
        CHECK(a2.enabled);
    }

    TEST(Disabling)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        anim.setEnabled(true);
        anim.setEnabled(false);

        CHECK(!anim.isEnabled());
        CHECK(!a1.enabled);
        CHECK(!a2.enabled);
    }

    TEST(AddComponentAnimationsToEnabledAnimation)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.setEnabled(true);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        CHECK(a1.enabled);
        CHECK(a2.enabled);
    }

    TEST(AddComponentAnimationsToDisabledAnimation)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.setEnabled(false);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        CHECK(!a1.enabled);
        CHECK(!a2.enabled);
    }

    TEST(Looping)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        anim.setLooping(false);
        anim.setLooping(true);

        CHECK(anim.isLooping());
        CHECK(a1.loop);
        CHECK(a2.loop);
    }

    TEST(NotLooping)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        anim.setLooping(true);
        anim.setLooping(false);

        CHECK(!anim.isLooping());
        CHECK(!a1.loop);
        CHECK(!a2.loop);
    }

    TEST(AddComponentAnimationsToLoopingAnimation)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.setLooping(true);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        CHECK(a1.loop);
        CHECK(a2.loop);
    }

    TEST(AddComponentAnimationsToNotLoopingAnimation)
    {
        MinMaxAnimation a1, a2;
        Animation anim("test");

        anim.setLooping(false);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);

        CHECK(!a1.loop);
        CHECK(!a2.loop);
    }

    TEST(UpdateOneComponentAnimation)
    {
        Animable animable;
        MinMaxAnimation a1(0.0f, 10.0f, 10.0f);
        Animation anim("test");

        anim.setEnabled(true);

        animable.animations.push_back(&a1);

        anim.addComponentAnimation(&a1);

        anim.reset();
        animable.update();
        CHECK_CLOSE(0.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(5.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(10.0f, animable.value, 1e-6f);
    }

    TEST(UpdateTwoComponentAnimationsWithSameLength)
    {
        Animable animable;
        MinMaxAnimation a1(0.0f, 10.0f, 10.0f);
        MinMaxAnimation a2(0.0f, 5.0f, 10.0f);
        Animation anim("test");

        anim.setEnabled(true);

        animable.animations.push_back(&a1);
        animable.animations.push_back(&a2);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);
        
        a1.setWeight(0.5f);
        a2.setWeight(0.5f);

        anim.reset();
        animable.update();
        CHECK_CLOSE(0.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(3.75f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(7.5f, animable.value, 1e-6f);
    }

    TEST(UpdateTwoComponentAnimationsWithSameLengthAndDifferentWeights)
    {
        Animable animable;
        MinMaxAnimation a1(0.0f, 10.0f, 10.0f);
        MinMaxAnimation a2(0.0f, 5.0f, 10.0f);
        Animation anim("test");

        anim.setEnabled(true);

        animable.animations.push_back(&a1);
        animable.animations.push_back(&a2);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);
        
        a1.setWeight(0.75f);
        a2.setWeight(0.25f);

        anim.reset();
        animable.update();
        CHECK_CLOSE(0.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(4.375f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(8.75f, animable.value, 1e-6f);
    }

    TEST(UpdateTwoComponentAnimationsWithDifferentLengths)
    {
        Animable animable;
        MinMaxAnimation a1(0.0f, 10.0f, 10.0f);
        MinMaxAnimation a2(0.0f, 5.0f, 5.0f);
        Animation anim("test");

        anim.setEnabled(true);

        animable.animations.push_back(&a1);
        animable.animations.push_back(&a2);

        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);
        
        a1.setWeight(0.5f);
        a2.setWeight(0.5f);

        anim.reset();
        animable.update();
        CHECK_CLOSE(0.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(5.0f, animable.value, 1e-6f);

        anim.update(5.0f);
        animable.update();
        CHECK_CLOSE(7.5f, animable.value, 1e-6f);
    }

    TEST(UpdateTwoComponentAnimationsWithDifferentLengthsAndOffset)
    {
        Animable animable;
        MinMaxAnimation a1(0.0f, 10.0f, 10.0f);
        MinMaxAnimation a2(0.0f, 5.0f, 5.0f);
        Animation anim("test");

        anim.setEnabled(true);

        animable.animations.push_back(&a1);
        animable.animations.push_back(&a2);

        a1.setWeight(0.5f);
        a2.setWeight(0.5f);
        a2.setOffset(2.5f);
        
        anim.addComponentAnimation(&a1);
        anim.addComponentAnimation(&a2);
        
        anim.reset();
        animable.update();
        CHECK_CLOSE(0.0f, animable.value, 1e-6f);

        anim.update(2.5f);
        animable.update();
        CHECK_CLOSE(1.25f, animable.value, 1e-6f);

        anim.update(2.5f);
        animable.update();
        CHECK_CLOSE(3.75f, animable.value, 1e-6f);

        anim.update(2.5f);
        animable.update();
        CHECK_CLOSE(6.25f, animable.value, 1e-6f);

        anim.update(2.5f);
        animable.update();
        CHECK_CLOSE(7.5f, animable.value, 1e-6f);
    }
}
