#pragma once

#include "ofMain.h"

class MoveCam : public ofCamera
{
public:
	MoveCam();
	~MoveCam();

	void setup();
	void update();

	// Mouse interactions
	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	// Key interactions
	void keyPressed(ofKeyEventArgs & args);

	//void begin() { camera.begin(); }
	//void end() { camera.end(); }

	void rotateToPoint(ofVec3f position);
	void setCamPosition(ofVec3f position);

	//ofVec3f getForward() { return camera.getLookAtDir(); }

protected:

	

	// Key activate events
	void registerEvent();

	//ofCamera	camera;
	ofNode		cameraBody;
	
	uint64_t	lastTime;

	float		delta;

	float		speed;
	float		prevRotX;
	float		prevRotY;
};

