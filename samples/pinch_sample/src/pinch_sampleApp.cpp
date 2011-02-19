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
    Matrix44f   mMatrix, mLastMatrix;
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
}

void pinch_sampleApp::update()
{
}

void pinch_sampleApp::draw()
{
    gl::clear(Color::black());
    
    gl::enableAlphaBlending();
    
//    gl::setMatrices(mCamera);
//    gl::multModelView(mMatrix);
//    
//    gl::rotate(Vec3f(30,40,30));
//    gl::color(Color::white());
//    gl::drawStrokedCube(Vec3f::zero(), Vec3f::one());
    gl::setMatricesWindow(getWindowSize(), true);
    gl::multModelView(mMatrix);
    
    gl::color(ColorA(1,1,1,0.25f));
    gl::drawSolidRect(getWindowBounds());
    
    gl::translate(getWindowCenter());
    gl::rotate(Vec3f(30,40,30));
    gl::scale(Vec3f(100,100,100));
    gl::color(Color::white());
    gl::drawStrokedCube(Vec3f::zero(), Vec3f::one());

}


bool pinch_sampleApp::onPinchBegan(PinchEvent event)
{
    mLastMatrix = mMatrix;
    return false;
}

bool pinch_sampleApp::onPinchMoved(PinchEvent event)
{
//    mMatrix = event.getTransform() * mLastMatrix;
    mMatrix = event.getTransformDelta() * mMatrix;
    return false;
}

bool pinch_sampleApp::onPinchEnded(PinchEvent event)
{
    return false;
}


CINDER_APP_COCOA_TOUCH( pinch_sampleApp, RendererGl )
