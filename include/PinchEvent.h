/*
 *  PinchEvent.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 */

#pragma once

#include "cinder/app/Event.h"
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "cinder/Quaternion.h"

namespace cinder { namespace app {

class PinchEvent : public Event {
private:
    
    Vec2f mTranslation;
    Vec2f mRotationOrigin, mScaleOrigin;
    
    float mRotation, mScale;
        
public:

    PinchEvent() : mRotation(0), mScale(0){}
    PinchEvent(const Vec2f &translation, const Vec2f &rotationOrigin, float rotation, const Vec2f &scaleOrigin, float scale)
    : mTranslation(translation), mRotationOrigin(rotationOrigin), mRotation(rotation), mScaleOrigin(scaleOrigin), mScale(scale)
    {}
    
//    const Vec2f& getOrigin()      const { return mOrigin; }
    const Vec2f& getTranslation() const { return mTranslation; }
    float        getRotation()    const { return mRotation; }
    float        getScale()       const { return mScale; }
    
    Quatf getQuat()
    {
        return Quatf(Vec3f::zAxis(), mRotation);
    }
    
    Matrix44f getTransform(const Matrix44f &obj_transform)
    {
        Vec3f rotationOrigin = obj_transform.transformPointAffine(Vec3f(mRotationOrigin));
        Matrix44f mtx;
        mtx.translate(Vec3f(mTranslation));
        mtx.translate(Vec3f(mScaleOrigin));
        mtx.scale(Vec3f(mScale, mScale, mScale));
        mtx.translate(-Vec3f(mScaleOrigin));
        mtx *= obj_transform;
        mtx.translate(Vec3f(mRotationOrigin));
        mtx.rotate(Vec3f::zAxis(), mRotation);
        mtx.translate(-Vec3f(mRotationOrigin));
        return mtx;
    }
    
};
	
} }