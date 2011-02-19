#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Camera.h"
#include "cinder/Matrix.h"

#include "PinchRecognizer.h"

using namespace ci;
using namespace ci::app;

class pinch_sampleApp : public AppCocoaTouch {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
    
    bool onPinchBegan(PinchEvent event);
    bool onPinchMoved(PinchEvent event);
    bool onPinchEnded(PinchEvent event);
    
    PinchRecognizer mPinchRecognizer;
    
    CameraPersp mCamera;
    Matrix44f   mMatrix;
    
    vector<TouchEvent::Touch> mPinchTouches;
};


void pinch_sampleApp::prepareSettings(Settings *settings)
{
    settings->enableMultiTouch();
}

void pinch_sampleApp::setup()
{
	mPinchRecognizer.init(this);
    mPinchRecognizer.registerBegan(this, &pinch_sampleApp::onPinchBegan);
    mPinchRecognizer.registerMoved(this, &pinch_sampleApp::onPinchMoved);
    mPinchRecognizer.registerEnded(this, &pinch_sampleApp::onPinchEnded);
    
    mCamera.lookAt(Vec3f(0,0,-5), Vec3f::zero(), Vec3f::yAxis());
    mCamera.setAspectRatio(getWindowAspectRatio());
    
    gl::enableDepthRead();
}

void pinch_sampleApp::update()
{
}

void pinch_sampleApp::draw()
{
    gl::clear(Color::black());
    
    gl::enableAlphaBlending();
    
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::color(Color(1,0,0));
    for(vector<TouchEvent::Touch>::iterator it = mPinchTouches.begin(); it != mPinchTouches.end(); ++it)
        gl::drawSolidCircle(it->getPos(), 30);
    
    gl::multModelView(mMatrix);
    
    gl::color(Color(0.25f, 0.25f, 0.25f));
    gl::drawSolidRect(getWindowBounds());
}


bool pinch_sampleApp::onPinchBegan(PinchEvent event)
{
    mPinchTouches = event.getTouches();
    return false;
}

bool pinch_sampleApp::onPinchMoved(PinchEvent event)
{
    mPinchTouches = event.getTouches();
    mMatrix = event.getTransformDelta() * mMatrix;
    return false;
}

bool pinch_sampleApp::onPinchEnded(PinchEvent event)
{
    mPinchTouches.clear();
    return false;
}


CINDER_APP_COCOA_TOUCH( pinch_sampleApp, RendererGl )
