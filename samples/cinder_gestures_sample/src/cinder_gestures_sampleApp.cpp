#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
//#include "TapAnalyzer.h"
#include "PinchAnalyzer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cinder_gestures_sampleApp : public AppBasic {
public:
	
	PinchAnalyzer mPinchAnalyzer;
//	TapAnalyzer   mTapAnalyzer;
	
	Matrix44f mRectTransform;  // transform of our rectangle
    Matrix44f mLastTransform;
	bool green; // toggled by double tap
	bool small; // toggled by single tap

	void setup();
	void prepareSettings( Settings *settings );
	void update();
	void draw();
	void resize();
	
	bool pinchBegan( PinchEvent event );	
	bool pinchMoved( PinchEvent event );	
	bool pinchEnded( PinchEvent event );	
	
//	bool singleTap( SingleTapEvent event );
//	bool doubleTap( DoubleTapEvent event );

};

void cinder_gestures_sampleApp::prepareSettings(Settings *settings)
{
	settings->enableMultiTouch(true);
	settings->setTitle("Cinder Gestures Test App");
	green = false;
	small = false;
}


void cinder_gestures_sampleApp::setup()
{	
	// center everything!
	mRectTransform.translate(Vec3f(getWindowWidth() / 2, getWindowHeight() / 2, 0));
	
	mPinchAnalyzer.init(this);
	mPinchAnalyzer.registerPinchBegan(this, &cinder_gestures_sampleApp::pinchBegan);
	mPinchAnalyzer.registerPinchMoved(this, &cinder_gestures_sampleApp::pinchMoved);
	mPinchAnalyzer.registerPinchEnded(this, &cinder_gestures_sampleApp::pinchEnded);
	
//	mTapAnalyzer.init(this);
//	mTapAnalyzer.registerSingleTap(this, &cinder_gestures_sampleApp::singleTap);
//	mTapAnalyzer.registerDoubleTap(this, &cinder_gestures_sampleApp::doubleTap);
}


bool cinder_gestures_sampleApp::pinchBegan( PinchEvent event )
{
	cout << "pinch began" << endl;
    mLastTransform = mRectTransform;
	return false;
}

bool cinder_gestures_sampleApp::pinchMoved( PinchEvent event )
{
	cout << "pinch moved" << endl;
	mRectTransform = mLastTransform * event.getTransform();
	return true; // consumed (does nothing yet)
}

bool cinder_gestures_sampleApp::pinchEnded( PinchEvent event )
{
	cout << "pinch ended" << endl;
    mRectTransform = mLastTransform * event.getTransform();
	return false;
}


//bool cinder_gestures_sampleApp::doubleTap( DoubleTapEvent event )
//{
//	// TODO: check location to make sure both taps were near the rectangle?
//	green = !green;
//	return false;
//}
//
//bool cinder_gestures_sampleApp::singleTap( SingleTapEvent event )
//{
//	// TODO: check location to make sure both taps were near the rectangle?
//	small = !small;
//	cout << "single tapped; small = " << small << endl;
//	return false;	
//}


void cinder_gestures_sampleApp::update()
{
	// needed for SingleTapEvents:
//	mTapAnalyzer.update();
}

void cinder_gestures_sampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::pushModelView();
	gl::multModelView( mRectTransform );
	
	float w = getWindowWidth();
	float h = getWindowHeight();
	
	float s = math<float>::min(w, h) / 2.0f;
	if (small) s /= 2.0f;
	
	gl::color( green ? Color(0, 1, 0) : Color(1, 0, 0) );
	gl::drawSolidRect( Rectf( -s/2.0f, -s/2.0f, s/2.0f, s/2.0f ) );
	
	gl::popModelView();
}


CINDER_APP_BASIC( cinder_gestures_sampleApp, RendererGl )
