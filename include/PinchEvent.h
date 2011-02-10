/*
 *  PinchEvent.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 *
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/Event.h"

namespace cinder { namespace app {

	class PinchEvent : public Event {
	private:
		bool mHasTransform;
		Vec2d mTranslate;
		double mScale;
		Vec2d mScaleOrigin;
		double mRotation;
		Vec2d mRotationOrigin;
	public:
		PinchEvent()
		{
			mHasTransform = false;
		}
		PinchEvent(const Vec2d &translate, const double &scale, const Vec2d &scaleOrigin, const double &rotation, const Vec2d &rotationOrigin): mTranslate(translate), mScale(scale), mScaleOrigin(scaleOrigin), mRotation(rotation), mRotationOrigin(rotationOrigin)
		{
			mHasTransform = true;
		}
		~PinchEvent() {}
		
		const Vec2d& getTranslation() { return mTranslate; }
		const double& getScale() { return mScale; }
		const Vec2d& getScaleOrigin() { return mScaleOrigin; }
		const double& getRotation() { return mRotation; }
		const Vec2d& getRotationOrigin() { return mRotationOrigin; }
		
		const bool& hasTransform() { return mHasTransform; }
		
		Matrix44d getTransform()
		{
			Matrix44d transform;
			transform.translate(Vec3d(mTranslate));
			transform.translate(-Vec3d(mScaleOrigin));
			transform.scale(Vec3d(mScale,mScale,mScale));
			transform.translate(Vec3d(mScaleOrigin));
			transform.translate(-Vec3d(mRotationOrigin));
			transform.rotate(Vec3d(0,0,1), mRotation);
			transform.translate(Vec3d(mRotationOrigin));
			return transform;
		}
	};
	
} }