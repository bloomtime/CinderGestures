#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "GestureHelper.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderGesturesApp : public AppBasic {
  public:
	
	GestureHelper mGestureHelper;
	Matrix44d mView; // manipulated by pinching
	bool on; // toggled by double tap

	void setup();
	void prepareSettings( Settings *settings );
	void update();
	void draw();
	
	bool pinchBegan( PinchEvent event );	
	bool pinchMoved( PinchEvent event );	
	bool pinchEnded( PinchEvent event );	
	bool doubleTap( DoubleTapEvent event );
};

void CinderGesturesApp::prepareSettings(Settings *settings)
{
	settings->enableMultiTouch(true);
	settings->setTitle("Cinder Gestures Test App");
	on = false;
}

void CinderGesturesApp::setup()
{	
	mView.translate(Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
	mGestureHelper.setup(this);
	mGestureHelper.registerPinchBegan(this, &CinderGesturesApp::pinchBegan);
	mGestureHelper.registerPinchMoved(this, &CinderGesturesApp::pinchMoved);
	mGestureHelper.registerPinchEnded(this, &CinderGesturesApp::pinchEnded);
	mGestureHelper.registerDoubleTap(this, &CinderGesturesApp::doubleTap);
}

bool CinderGesturesApp::pinchBegan( PinchEvent event )
{
	cout << "pinch began" << endl;
	return false;
}

bool CinderGesturesApp::pinchMoved( PinchEvent event )
{
	cout << "pinch moved" << endl;
	
	mView.translate(Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
	mView *= event.getTransform();
	mView.translate(-Vec3f(getWindowWidth()/2,getWindowHeight()/2,0));
		
	return true; // consumed (does nothing yet)
}

bool CinderGesturesApp::pinchEnded( PinchEvent event )
{
	cout << "pinch ended" << endl;
	return false;
}

bool CinderGesturesApp::doubleTap( DoubleTapEvent event )
{
	// TODO: check location to make sure both taps were near the rectangle?
	on = !on;
}

void CinderGesturesApp::update()
{
}

void CinderGesturesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::pushModelView();
	gl::multModelView( mView );
	
	double w = getWindowWidth();
	double h = getWindowHeight();
	
	gl::color( Color(on ? 0 : 1, on ? 1 : 0, 0) );
	//gl::drawSolidRect( Rectf( w/4.0f, h/4.0f, 3.0f*w/4.0f, 3.0f*h/4.0f ) );
	gl::drawSolidRect( Rectf( -w/4.0f, -h/4.0f, w/4.0f, h/4.0f ) );
	
	gl::popModelView();
}


CINDER_APP_BASIC( CinderGesturesApp, RendererGl )
