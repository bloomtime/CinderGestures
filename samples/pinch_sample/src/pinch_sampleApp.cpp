#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Matrix.h"
#include "cinder/Rect.h"
#include "cinder/Rand.h"

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

    struct TouchRect {
        Matrix44f mMatrix;
        Rectf     mBounds;
        Color     mColor;
        
        TouchRect(const Rectf &bounds, const Vec3f &pos, float rot){
            mBounds = bounds;
            mMatrix.translate(pos);
            mMatrix.rotate(Vec3f::zAxis(), rot);
            mColor.set(CM_HSV, Vec3f(Rand::randFloat(), Rand::randFloat(), Rand::randFloat()));
        }

        bool getIsHit(const TouchEvent::Touch &touch){
            return mBounds.contains((mMatrix.inverted() * Vec3f(touch.getPos(), 0.0f)).xy());
        }
        
        void draw(){
            gl::pushModelView();
            gl::multModelView(mMatrix);
            gl::color(mColor);
            gl::drawSolidRect(mBounds);
            gl::popModelView();
        }
    };
    
    TouchRect                *mPinchingRect;
    
    vector<TouchRect>         mRects;
    vector<TouchEvent::Touch> mPinchTouches;
};


void pinch_sampleApp::prepareSettings(Settings *settings)
{
    settings->enableMultiTouch();
}

void pinch_sampleApp::setup()
{
    mPinchingRect = 0;
    
	mPinchRecognizer.init(this);
    mPinchRecognizer.registerBegan(this, &pinch_sampleApp::onPinchBegan);
    mPinchRecognizer.registerMoved(this, &pinch_sampleApp::onPinchMoved);
    mPinchRecognizer.registerEnded(this, &pinch_sampleApp::onPinchEnded);
    
    Rand::randomize();
    
    for(int i = 3; --i >= 0;){
        float scale = Rand::randFloat(100, 200);
        mRects.push_back(TouchRect(
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
    
    gl::enableAlphaBlending();
    
    gl::setMatricesWindow(getWindowSize(), true);
    
    for(vector<TouchRect>::iterator it = mRects.begin(); it != mRects.end(); ++it)
        it->draw();
    
    gl::color(Color(1,0,0));
    for(vector<TouchEvent::Touch>::iterator it = mPinchTouches.begin(); it != mPinchTouches.end(); ++it)
        gl::drawSolidCircle(it->getPos(), 30);
}


bool pinch_sampleApp::onPinchBegan(PinchEvent event)
{
    mPinchTouches = event.getTouches();
    console() << mPinchTouches.size();
    for(vector<TouchRect>::reverse_iterator it = mRects.rbegin(); it != mRects.rend(); ++it){
        if(it->getIsHit(mPinchTouches[0]) && it->getIsHit(mPinchTouches[1])){
            mPinchingRect = &(*it);
            return true;
        }
    }
    return false;
}

bool pinch_sampleApp::onPinchMoved(PinchEvent event)
{
    mPinchTouches = event.getTouches();
    if(mPinchingRect){
        mPinchingRect->mMatrix = event.getTransformDelta() * mPinchingRect->mMatrix;
        return true;
    }
    return false;
}

bool pinch_sampleApp::onPinchEnded(PinchEvent event)
{
    mPinchTouches.clear();
    mPinchingRect = 0;
    return false;
}


CINDER_APP_COCOA_TOUCH( pinch_sampleApp, RendererGl )
