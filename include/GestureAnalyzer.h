/*
 *  GestureAnalyzer.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 *
 */

#pragma once

#include "cinder/Cinder.h"

namespace cinder { namespace app {

	class GestureAnalyzer {

	protected:
		
		// your Cinder App (usually passed in as 'this' in setup)
		AppBasic *mApp;
				
		// subclasses must handle basic touch events from mApp...
		virtual bool touchesBegan(TouchEvent event) {}
		virtual bool touchesMoved(TouchEvent event) {}
		virtual bool touchesEnded(TouchEvent event) {}
		
		// for cleanup in destructor/setup, keep callback ids
		CallbackId mTouchesBeganCb;
		CallbackId mTouchesMovedCb;
		CallbackId mTouchesEndedCb;
				
	public:		
		
		GestureAnalyzer() {
			mApp = NULL;
		}
		
		virtual ~GestureAnalyzer() {
			if (mApp != NULL) {
				mApp->unregisterTouchesBegan(mTouchesBeganCb);
				mApp->unregisterTouchesMoved(mTouchesMovedCb);
				mApp->unregisterTouchesEnded(mTouchesEndedCb);
				mApp = NULL;
			}		
		}
		
		virtual void setup(AppBasic *app) {
			if (mApp != NULL) {
				mApp->unregisterTouchesBegan(mTouchesBeganCb);
				mApp->unregisterTouchesMoved(mTouchesMovedCb);
				mApp->unregisterTouchesEnded(mTouchesEndedCb);
				mApp = NULL;
			}
			mApp = app;
			if (mApp != NULL) {
				mTouchesBeganCb = mApp->registerTouchesBegan(this, &GestureAnalyzer::touchesBegan);
				mTouchesMovedCb = mApp->registerTouchesMoved(this, &GestureAnalyzer::touchesMoved);
				mTouchesEndedCb = mApp->registerTouchesEnded(this, &GestureAnalyzer::touchesEnded);
			}
		}
				
	};
	
} }
