#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "DoubleTapAnalyzer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cinder_gestures_sampleApp : public AppBasic {
  public:
	
//	GestureHelper mGestureHelper;
	DoubleTapAnalyzer mDoubleTapAnalyzer;
	Matrix44d mView; // manipulated by pinching
	bool on; // toggled by double tap

	void setup();
	void prepareSettings( Settings *settings );
	void update();
	void draw();
	void resize();
	
//	bool pinchBegan( PinchEvent event );	
//	bool pinchMoved( PinchEvent event );	
//	bool pinchEnded( PinchEvent event );	
	bool doubleTap( DoubleTapEvent event );
};

void cinder_gestures_sampleApp::prepareSettings(Settings *settings)
{
	settings->enableMultiTouch(true);
	settings->setTitle("Cinder Gestures Test App");
	on = false;
}

void cinder_gestures_sampleApp::setup()
{	
	// center everything!
	mView.translate(Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
	
//	mGestureHelper.setup(this);
//	mGestureHelper.registerPinchBegan(this, &cinder_gestures_sampleApp::pinchBegan);
//	mGestureHelper.registerPinchMoved(this, &cinder_gestures_sampleApp::pinchMoved);
//	mGestureHelper.registerPinchEnded(this, &cinder_gestures_sampleApp::pinchEnded);
	mDoubleTapAnalyzer.setup(this);
	mDoubleTapAnalyzer.registerDoubleTap(this, &cinder_gestures_sampleApp::doubleTap);
}

//bool cinder_gestures_sampleApp::pinchBegan( PinchEvent event )
//{
//	cout << "pinch began" << endl;
//	return false;
//}
//
//bool cinder_gestures_sampleApp::pinchMoved( PinchEvent event )
//{
//	cout << "pinch moved" << endl;
//	
//	mView.translate(Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
//	mView *= event.getTransform();
//	mView.translate(-Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
//		
//	return true; // consumed (does nothing yet)
//}
//
//bool cinder_gestures_sampleApp::pinchEnded( PinchEvent event )
//{
//	cout << "pinch ended" << endl;
//	return false;
//}

bool cinder_gestures_sampleApp::doubleTap( DoubleTapEvent event )
{
	// TODO: check location to make sure both taps were near the rectangle?
	on = !on;
}

void cinder_gestures_sampleApp::update()
{
}

void cinder_gestures_sampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::pushModelView();
	gl::multModelView( mView );
	
	double w = getWindowWidth();
	double h = getWindowHeight();
	
	gl::color( Color(on ? 0 : 1, on ? 1 : 0, 0) );
	gl::drawSolidRect( Rectf( -w/4.0f, -h/4.0f, w/4.0f, h/4.0f ) );
	
	gl::popModelView();
}


CINDER_APP_BASIC( cinder_gestures_sampleApp, RendererGl )
