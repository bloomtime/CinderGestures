/*
 *  GestureAnalyzer.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 */

#pragma once

#include "cinder/Cinder.h"
#if defined (CINDER_COCOA_TOUCH)
#include "cinder/app/AppCocoaTouch.h"
#else
#include "cinder/app/AppBasic.h"
#endif

namespace cinder { namespace app {

class GestureAnalyzer {
protected:
    
#if defined (CINDER_COCOA_TOUCH)
    typedef AppCocoaTouch AppType;
#else
    typedef AppBasic AppType;
#endif
    
    // your Cinder App (usually passed in as 'this' in setup)
    AppType *mApp;
            
    // subclasses must handle basic touch events from mApp...
    virtual bool touchesBegan(TouchEvent event) = 0;
    virtual bool touchesMoved(TouchEvent event) = 0;
    virtual bool touchesEnded(TouchEvent event) = 0;
    
    // for cleanup in destructor/setup, keep callback ids
    CallbackId mTouchesBeganCb;
    CallbackId mTouchesMovedCb;
    CallbackId mTouchesEndedCb;
    
    void registerTouchCallbacks(){
        if(mApp){
            mTouchesBeganCb = mApp->registerTouchesBegan(this, &GestureAnalyzer::touchesBegan);
            mTouchesMovedCb = mApp->registerTouchesMoved(this, &GestureAnalyzer::touchesMoved);
            mTouchesEndedCb = mApp->registerTouchesEnded(this, &GestureAnalyzer::touchesEnded);
        }
    }
    void unregisterTouchCallbacks(){
        if(mApp){
            mApp->unregisterTouchesBegan(mTouchesBeganCb);
            mApp->unregisterTouchesMoved(mTouchesMovedCb);
            mApp->unregisterTouchesEnded(mTouchesEndedCb);
        }
    }
            
public:		
    
    GestureAnalyzer() : mApp(0) {}
    
    ~GestureAnalyzer(){
        unregisterTouchCallbacks();
    }
    
    virtual void init(AppType *app)
    {
        unregisterTouchCallbacks();
        mApp = app;
        registerTouchCallbacks();
    }
            
};
	
} }
