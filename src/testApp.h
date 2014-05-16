#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxTLCameraTrack.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void draw();
		void update();
		void audioIn(float * input, int bufferSize, int nChannels);
		void cube(float x, float y, float z, float s, int n,ofColor colors);
		void keyPressed(int key);
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;
		int     bufferCounter;
		int     drawCounter;
		float smoothedVol;
		float scaledVol;
		ofSoundStream soundStream;
		ofxTimeline timeline;
	        ofxTLCameraTrack* cameraTrack;
	        ofEasyCam cam;
	        vector<ofVec3f> boxes;
	        ofRectangle makeCameraPoint;
	        ofRectangle toggleLockTrack;
};
