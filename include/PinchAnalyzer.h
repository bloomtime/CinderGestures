/*
 *  PinchAnalyzer.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 *
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/TouchEvent.h"
#include "GestureAnalyzer.h"
#include "PinchEvent.h"

namespace cinder { namespace app {

	class PinchAnalyzer : public GestureAnalyzer {

	private:
		
		// keep track of previous touch point locations by id
		std::map<uint32_t, Vec2f> mTouchesById;
		
		// are we pinching yet?
		bool mPinching;
		
		// handle basic touch events from mApp...
		bool touchesBegan(TouchEvent event);
		bool touchesMoved(TouchEvent event);
		bool touchesEnded(TouchEvent event);
		
		// keep track of listeners for our own events...
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchBegan;
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchMoved;
		CallbackMgr<bool(PinchEvent)> mCallbacksPinchEnded;
				
	public:		
		
		PinchAnalyzer(): GestureAnalyzer() {
			mPinching = false;
		}
		
		void setup(AppBasic *app) {
			GestureAnalyzer::setup(app);
			mTouchesById.clear();
			mPinching = false;
		}
		
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
		
	};

	bool PinchAnalyzer::touchesBegan(TouchEvent event)
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
	
	bool PinchAnalyzer::touchesMoved(TouchEvent event)
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
	
	bool PinchAnalyzer::touchesEnded(TouchEvent event)
	{
		std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
		if (mPinching && touches.size() < 2) {
			mCallbacksPinchEnded.call(PinchEvent());
			mPinching = false;
		}
		return false;
	}
	
} }
