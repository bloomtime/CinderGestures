#pragma once

#include "cinder/app/Event.h"
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "cinder/Quaternion.h"

namespace cinder { namespace app {


class PinchEvent : public Event {
private:
    
    Vec2f mTouch1Start, mTouch2Start;
    Vec2f mTouch1Prev,  mTouch2Prev;
    Vec2f mTouch1,      mTouch2;
    
public:

    PinchEvent(){}
    PinchEvent(const Vec2f &t1s, const Vec2f &t2s, const Vec2f &t1p, const Vec2f &t2p, const Vec2f &t1, const Vec2f &t2)
    : mTouch1Start(t1s), mTouch2Start(t2s), mTouch1Prev(t1p), mTouch2Prev(t2p), mTouch1(t1), mTouch2(t2)
    {}

    Vec2f getTranslation() const {
        return mTouch1 - mTouch1Start;
    }
    Vec2f getTranslationDelta() const {
        return mTouch1 - mTouch1Prev;
    }

    float getRotation() const {
        return math<float>::atan2(mTouch2.y - mTouch1.y, mTouch2.x - mTouch1.x) - math<float>::atan2(mTouch2Start.y - mTouch1Start.y, mTouch2Start.x - mTouch1Start.x);
    }
    float getRotationDelta() const {
        return math<float>::atan2(mTouch2.y - mTouch1.y, mTouch2.x - mTouch1.x) - math<float>::atan2(mTouch2Prev.y - mTouch1Prev.y, mTouch2Prev.x - mTouch1Prev.x);
    }

    float getScale() const {
        return mTouch1.distance(mTouch2) / mTouch1Start.distance(mTouch2Start);
    }
    float getScaleDelta() const {
        return mTouch1.distance(mTouch2) / mTouch1Prev.distance(mTouch2Prev);
    }
    
    Matrix44f getTransform()
    {
        float scale = getScale();

        Matrix44f mtx;
        mtx.translate(Vec3f(mTouch1, 0.0f));
        mtx.rotate(Vec3f::zAxis(), getRotation());
        mtx.scale(Vec3f(scale, scale, scale));
        mtx.translate(Vec3f(getTranslation() - mTouch1, 0.0f));
        return mtx;
    }
                   
    Matrix44f getTransformDelta()
    {
        float scale = getScaleDelta();
        
        Matrix44f mtx;
        mtx.translate(Vec3f(mTouch1, 0.0f));
        mtx.rotate(Vec3f::zAxis(), getRotationDelta());
        mtx.scale(Vec3f(scale, scale, scale));
        mtx.translate(Vec3f(getTranslationDelta() - mTouch1, 0.0f));
        return mtx;
    }
    
};

                      
} } // namespace cinder::app