#pragma once

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/Matrix.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/Rand.h"

#include "PinchRecognizer.h"
#include "TapRecognizer.h"

using namespace ci;

class TouchRect {
public:

    PinchRecognizer mPinchRecognizer;
    TapRecognizer   mTapRecognizer;

    Matrix44f mMatrix;
    Rectf     mBounds;
    Color     mColor;
    
    float     mFlipRotation, mFlipRotationGoal;
    bool      mIsPinching;
    
    bool pinchBegan(PinchEvent event)
    {
        mIsPinching = true;
        return true;
    }
    bool pinchMoved(PinchEvent event)
    {
        if(mIsPinching){
            mMatrix = event.getTransformDelta() * mMatrix;
            return true;
        }
        return false;
    }
    bool pinchEnded(PinchEvent event)
    {
        mIsPinching = false;
        return false;
    }
    
    bool doubleTapped(DoubleTapEvent event)
    {
        mFlipRotationGoal = mFlipRotationGoal > 0 ? 0 : M_PI;
        return true;
    }
    
    bool getIsHit(TouchEvent::Touch touch)
    {
        return mBounds.contains((mMatrix.inverted() * Vec3f(touch.getPos(), 0.0f)).xy());
    }
    
    TouchRect(app::AppCocoaTouch *app, const Rectf &bounds, const Vec3f &pos, float rot)
    : mIsPinching(false), mFlipRotation(0), mFlipRotationGoal(0)
    {
        mBounds = bounds;
        mMatrix.translate(pos);
        mMatrix.rotate(Vec3f::zAxis(), rot);
        mColor.set(CM_HSV, Vec3f(Rand::randFloat(), Rand::randFloat(), 1));
        
        mTapRecognizer.init(app);
        mTapRecognizer.registerDoubleTap(this, &TouchRect::doubleTapped);
        mTapRecognizer.setKeepTouchCallback(this, &TouchRect::getIsHit);
        
        mPinchRecognizer.init(app);
        mPinchRecognizer.registerBegan(this, &TouchRect::pinchBegan);
        mPinchRecognizer.registerMoved(this, &TouchRect::pinchMoved);
        mPinchRecognizer.registerEnded(this, &TouchRect::pinchEnded);
        mPinchRecognizer.setKeepTouchCallback(this, &TouchRect::getIsHit);
    }
    
    void draw()
    {
        mFlipRotation = lerp(mFlipRotation, mFlipRotationGoal, 0.1f);
        gl::pushModelView();
        gl::multModelView(mMatrix * Matrix44f::createRotation(Vec3f::yAxis(), mFlipRotation));
        gl::color(mColor);
        gl::drawSolidRect(mBounds);
        gl::popModelView();
    }

};
