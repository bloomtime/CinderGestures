#pragma once

#include "cinder/app/Event.h"
#include "cinder/Vector.h"
#include "cinder/Ray.h"
#include "cinder/Matrix.h"
#include "cinder/Quaternion.h"

#include <ostream>

using std::vector;

namespace cinder { namespace app {


class PinchEvent : public Event {
private:
    
    Vec2f mTouch1Start, mTouch2Start;
    Vec2f mTouch1Prev,  mTouch2Prev;
    Vec2f mTouch1,      mTouch2;
    
    Vec2f mScreenSize;
    
    vector<TouchEvent::Touch> mTouches;
    
    static Vec3f calcRayPlaneIntersection(const Ray &ray, const Vec3f &planeOrigin, const Vec3f &planeNormal)
    {
        float denom = planeNormal.dot(ray.getDirection());
        float u = planeNormal.dot(planeOrigin - ray.getOrigin()) / denom;
        return ray.calcPosition(u);
    }
    
public:

    PinchEvent()
    {
    }
    
    PinchEvent(const vector<TouchEvent::Touch> &touches, const Vec2f &t1s, const Vec2f &t2s, const Vec2f &t1p, const Vec2f &t2p, const Vec2f &screenSize)
    : mTouch1Start(t1s), mTouch2Start(t2s),
      mTouch1Prev(t1p),  mTouch2Prev(t2p),
      mTouches(touches), mTouch1(touches[0].getPos()), mTouch2(touches[1].getPos()),
      mScreenSize(screenSize)
    {
    }

    Vec2f getTranslation() const {
        return mTouch1 - mTouch1Start;
    }
    Vec2f getTranslationDelta() const {
        return mTouch1 - mTouch1Prev;
    }

    float getRotation() const {
        return math<float>::atan2(mTouch2.y - mTouch1.y, mTouch2.x - mTouch1.x)
             - math<float>::atan2(mTouch2Start.y - mTouch1Start.y, mTouch2Start.x - mTouch1Start.x);
    }
    float getRotationDelta() const {
        return math<float>::atan2(mTouch2.y - mTouch1.y, mTouch2.x - mTouch1.x)
             - math<float>::atan2(mTouch2Prev.y - mTouch1Prev.y, mTouch2Prev.x - mTouch1Prev.x);
    }

    float getScale() const {
        return mTouch1.distance(mTouch2) / mTouch1Start.distance(mTouch2Start);
    }
    float getScaleDelta() const {
        return mTouch1.distance(mTouch2) / mTouch1Prev.distance(mTouch2Prev);
    }
    
    const vector<TouchEvent::Touch>& getTouches() const {
        return mTouches;
    }
    
    vector<Ray> getTouchRays(const Camera &cam) const {
        vector<Ray> touch_rays;
        for(vector<TouchEvent::Touch>::const_iterator it = mTouches.begin(); it != mTouches.end(); ++it){
            Vec2f pos = it->getPos();
            touch_rays.push_back(cam.generateRay(pos.x / mScreenSize.x, 1.0f - pos.y / mScreenSize.y, cam.getAspectRatio()));
        }
        return touch_rays;
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
    
    Matrix44f getTransformDelta(const Camera &cam, float depth)
    {
        Ray t1Ray  = cam.generateRay(mTouch1.x / mScreenSize.x, 1.0f - mTouch1.y / mScreenSize.y, cam.getAspectRatio());
        Ray t1pRay = cam.generateRay(mTouch1Prev.x / mScreenSize.x, 1.0f - mTouch1Prev.y / mScreenSize.y, cam.getAspectRatio());
        Vec3f planeOrigin(cam.getEyePoint() + cam.getViewDirection() * depth);
        Vec3f planeNormal(cam.getViewDirection() * -1.0f);
        Vec3f t1Pos  = calcRayPlaneIntersection(t1Ray, planeOrigin, planeNormal);
        Vec3f t1pPos = calcRayPlaneIntersection(t1pRay, planeOrigin, planeNormal);
        
        float scale = getScaleDelta();
        
        Matrix44f mtx;
        mtx.translate(t1Pos);
        mtx.rotate(cam.getViewDirection(), getRotationDelta());
        mtx.scale(Vec3f(scale, scale, scale));
        mtx.translate(t1Pos - t1pPos - t1Pos);
        return mtx;
    }
    
};

                      
} } // namespace cinder::app