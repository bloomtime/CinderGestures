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

namespace cinder { namespace app {

class PinchEvent : public Event {
private:
    
    Vec2f mTranslation;
    Vec2f mOrigin;
    
    float mRotation, mScale;
        
public:

    PinchEvent() : mRotation(0), mScale(0){}
    PinchEvent(const Vec2f &origin, const Vec2f &translation, float rotation, float scale)
    : mOrigin(origin), mTranslation(translation), mRotation(rotation), mScale(scale)
    {}
    
    const Vec2f& getOrigin()      const { return mOrigin; }
    const Vec2f& getTranslation() const { return mTranslation; }
    float        getRotation()    const { return mRotation; }
    float        getScale()       const { return mScale; }
    
    Matrix44f getTransform(const Matrix44f &obj_transform)
    {
        Vec3f origin(mOrigin);
        Vec3f translation(mTranslation);
        Matrix44f mtx;
        mtx.translate(translation);
        mtx *= obj_transform;
        mtx.translate(-origin);
        mtx.scale(Vec3f(mScale, mScale, mScale));
        mtx.translate(origin);
//        mtx.translate(-origin);
//        mtx.rotate(Vec3f::zAxis(), mRotation);
//        mtx.translate(origin);
        return mtx;
    }
    
};
	
} }