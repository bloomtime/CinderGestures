/*
 *  GestureHelper.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 *
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/TouchEvent.h"
#include "PinchEvent.h"
#include "DoubleTapEvent.h"

namespace cinder { namespace app {

	class GestureHelper {

	private:
		
		// your Cinder App (usually passed in as 'this' in setup)
		AppBasic *mApp;
		
		// keep track of previous touch point locations by id
		std::map<uint32_t, Vec2f> mTouchesById;
		
		// keep track of the touches from the last touch ended event, for double tap
		Vec2f prevTouchPos;
		double prevTouchTime;
		
		// are we pinching yet?
		bool mPinching;
		
		// are we twisting yet?
		bool mTwisting;
		
		// handle basic touch events from mApp...
		bool touchesBegan(TouchEvent event);
		bool touchesMoved(TouchEvent event);
		bool touchesEnded(TouchEvent event);
		
		// for cleanup in destructor/setup, keep callback ids
		CallbackId mTouchesBeganCb;
		CallbackId mTouchesMovedCb;
		CallbackId mTouchesEndedCb;

		// keep track of listeners for our own events...
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchBegan;
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchMoved;
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchEnded;
		CallbackMgr<bool(DoubleTapEvent)> mCallbacksDoubleTap;
				
	public:		
		
		GestureHelper() {
			mPinching = false;
			mTwisting = false;
			mApp = NULL;
		}
		
		~GestureHelper() {
			if (mApp != NULL) {
				mApp->unregisterTouchesBegan(mTouchesBeganCb);
				mApp->unregisterTouchesMoved(mTouchesMovedCb);
				mApp->unregisterTouchesEnded(mTouchesEndedCb);
				mApp = NULL;
			}			
		}
		
		void setup(AppBasic *app) {
			if (mApp != NULL) {
				mApp->unregisterTouchesBegan(mTouchesBeganCb);
				mApp->unregisterTouchesMoved(mTouchesMovedCb);
				mApp->unregisterTouchesEnded(mTouchesEndedCb);
				mApp = NULL;
			}
			mApp = app;
			if (mApp != NULL) {
				mApp->registerTouchesBegan(this, &GestureHelper::touchesBegan);
				mApp->registerTouchesMoved(this, &GestureHelper::touchesMoved);
				mApp->registerTouchesEnded(this, &GestureHelper::touchesEnded);
			}
			prevTouchTime = 0;
			prevTouchPos = Vec2f(0,0);
			mTouchesById.clear();
			mPinching = false;
			mTwisting = false;
		}
		
		/* 
		 * these registerXXX methods are convenience functions 
		 * so that you can type:
		 *
		 *     gestureHelper.registerPinchBegan(this, &MyApp::pinchBegan);
		 *
		 * rather than:
		 *
		 *     gestureHelper.mCallbacksPinchBegan.registerCb(std::bind1st(std::mem_fun(&MyApp::pinchBegan), this));
		 *     
		 * where pinchBegan is a function in your app:
		 *
		 *     bool MyApp::pinchBegan(PinchEvent event) { ... }
		 *
		 */
		template<typename T>
		CallbackId registerPinchBegan( T *obj, bool (T::*callback)(PinchEvent) ){
			return mCallbacksPinchBegan.registerCb(std::bind1st(std::mem_fun(callback), obj));
		}
		template<typename T>
		CallbackId registerPinchMoved( T *obj, bool (T::*callback)(PinchEvent) ){
			return mCallbacksPinchMoved.registerCb(std::bind1st(std::mem_fun(callback), obj));
		}
		template<typename T>
		CallbackId registerPinchEnded( T *obj, bool (T::*callback)(PinchEvent) ){
			return mCallbacksPinchEnded.registerCb(std::bind1st(std::mem_fun(callback), obj));
		}
		template<typename T>
		CallbackId registerDoubleTap( T *obj, bool (T::*callback)(DoubleTapEvent) ){
			return mCallbacksDoubleTap.registerCb(std::bind1st(std::mem_fun(callback), obj));
		}
		
	};

	bool GestureHelper::touchesBegan(TouchEvent event)
	{
		std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
		for (int i = 0; i < touches.size(); i++) {
			mTouchesById[touches[i].getId()] = touches[i].getPos();
		}
		if (!mPinching && touches.size() >= 2) {
			mPinching = true;
			mCallbacksPinchBegan.call(PinchEvent());			
		}
		return false;
	}
	
	bool GestureHelper::touchesMoved(TouchEvent event)
	{
		std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
		if (mPinching) {
			if (touches.size() == 2) {
				Vec2f p0 = touches[0].getPos();
				Vec2f p1 = touches[1].getPos();
				Vec2f p2 = mTouchesById[touches[0].getId()];
				Vec2f p3 = mTouchesById[touches[1].getId()];

				double startDist = p0.distance(p1);
				Vec2f startCenter = p0.lerp(0.5, p1);
				double startRot = atan2(p1.y-p0.y,p1.x-p0.x);

				double endDist = p2.distance(p3);
				Vec2f endCenter = p2.lerp(0.5, p3);
				double endRot = atan2(p3.y-p2.y,p3.x-p2.x);

				double sc = startDist / endDist;
				double r = startRot - endRot;

				mCallbacksPinchMoved.call(PinchEvent(startCenter-endCenter,sc,endCenter,r,endCenter));				
			}
		}
		for (int i = 0; i < touches.size(); i++) {
			mTouchesById[touches[i].getId()] = touches[i].getPos();
		}		
		return false;
	}
	
	bool GestureHelper::touchesEnded(TouchEvent event)
	{
		std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
		if (mPinching && touches.size() < 2) {
			mCallbacksPinchEnded.call(PinchEvent());
			mPinching = false;
		}
		TouchEvent::Touch thisTouch = event.getTouches()[0];
		double dt = thisTouch.getTime() - prevTouchTime;
		bool doubleTapped = false;
		if (dt > 0.05f && dt < 0.25f) {
			if (prevTouchPos.distance(thisTouch.getPos()) < 25.0f) {
				doubleTapped = true;
				mCallbacksDoubleTap.call(DoubleTapEvent(thisTouch.getPos(), thisTouch.getTime()));
			}
		}
		if (doubleTapped) {
			prevTouchPos = Vec2f(0,0);
			prevTouchTime = 0;
		}
		else {
			prevTouchPos = event.getTouches()[0].getPos();
			prevTouchTime = event.getTouches()[0].getTime();
		}
		return false;
	}
	
} }
