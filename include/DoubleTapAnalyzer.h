/*
 *  DoubleTapAnalyzer.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 *
 */

#pragma once

#include "GestureAnalyzer.h"
#include "DoubleTapEvent.h"

namespace cinder { namespace app {

	class DoubleTapAnalyzer : public GestureAnalyzer {

	protected:
				
		// keep track of the touches from the last touch ended event
		Vec2f prevTouchPos;
		double prevTouchTime;
		
		// override touch end handling from GestureAnalyzer base...
		bool touchesEnded(TouchEvent event);
		
		// keep track of listeners for our own events...
		CallbackMgr<bool(DoubleTapEvent)> mCallbacksDoubleTap;
				
	public:		
		
		void setup(AppBasic *app) {
			GestureAnalyzer::setup(app);
			prevTouchTime = 0;
			prevTouchPos = Vec2f(0,0);
		}
		
		/* 
		 * these registerXXX methods are convenience functions 
		 * so that you can type:
		 *
		 *     doubleTapAnalyzer.registerDoubleTap(this, &MyApp::doubleTap);
		 *
		 * rather than:
		 *
		 *     doubleTapAnalyzer.mCallbacksDoubleTap.registerCb(std::bind1st(std::mem_fun(&MyApp::doubleTap), this));
		 *     
		 * where doubleTap is a function in your app:
		 *
		 *     bool MyApp::doubleTap(DoubleTapEvent event) { ... }
		 *
		 */
		template<typename T>
		CallbackId registerDoubleTap( T *obj, bool (T::*callback)(DoubleTapEvent) ){
			return mCallbacksDoubleTap.registerCb(std::bind1st(std::mem_fun(callback), obj));
		}
		
	};
	
	bool DoubleTapAnalyzer::touchesEnded(TouchEvent event)
	{
		std::vector<TouchEvent::Touch> touches = event.getTouches();
		TouchEvent::Touch thisTouch = touches[0];
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
			prevTouchPos = touches[0].getPos();
			prevTouchTime = touches[0].getTime();
		}
		return false;
	}
	
} }
