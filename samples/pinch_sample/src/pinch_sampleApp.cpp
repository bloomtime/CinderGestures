#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "TouchRect.h"

using namespace ci;
using namespace ci::app;

class pinch_sampleApp : public AppCocoaTouch {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
    
//    bool onPinchBegan(PinchEvent event);
//    bool onPinchMoved(PinchEvent event);
//    bool onPinchEnded(PinchEvent event);
    
    vector<TouchRect*>        mRects;
    vector<TouchEvent::Touch> mPinchTouches;
    
    gl::Texture mRectTex;
};


void pinch_sampleApp::prepareSettings(Settings *settings)
{
    settings->enableMultiTouch();
}

void pinch_sampleApp::setup()
{
    gl::Texture::Format format;
    format.setMinFilter(GL_NEAREST);
    format.setMagFilter(GL_NEAREST);
    mRectTex = gl::Texture(loadImage(loadResource("xy.png")), format);
    
    Rand::randomize();
    
    for(int i = 3; --i >= 0;){
        float scale = Rand::randFloat(150, 200);
        mRects.push_back(new TouchRect(this,
            Rectf(-scale, -scale, scale, scale),
            Vec3f(Rand::randFloat(100, getWindowWidth() - 100), Rand::randFloat(100, getWindowWidth() - 100), 0),
            Rand::randFloat(M_PI * 2)
        ));
    }
}

void pinch_sampleApp::update()
{
}

void pinch_sampleApp::draw()
{
    gl::clear(Color::black());
    
    gl::setMatricesWindow(getWindowSize(), true);
    
    mRectTex.enableAndBind();
    for(vector<TouchRect*>::iterator it = mRects.begin(); it != mRects.end(); ++it)
        (*it)->draw();
    mRectTex.unbind();
    
//    gl::color(Color(1,0,0));
//    for(vector<TouchEvent::Touch>::iterator it = mPinchTouches.begin(); it != mPinchTouches.end(); ++it)
//        gl::drawSolidCircle(it->getPos(), 30);
}


//bool pinch_sampleApp::onPinchBegan(PinchEvent event)
//{
//    return false;
//}
//
//bool pinch_sampleApp::onPinchMoved(PinchEvent event)
//{
//    return false;
//}
//
//bool pinch_sampleApp::onPinchEnded(PinchEvent event)
//{
//    return false;
//}


CINDER_APP_COCOA_TOUCH( pinch_sampleApp, RendererGl )
