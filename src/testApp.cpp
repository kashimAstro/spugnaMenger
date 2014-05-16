#include "testApp.h"

bool drawTimeLine, shaderoff;
float xr;
float yr;
int stages;
int width,height;
ofLight light;
ofMaterial material;
ofEasyCam cam;
ofImage matCap;
ofShader shader;

void testApp::audioIn(float * input, int bufferSize, int nChannels){
        float curVol = 0.0;
        int numCounted = 0;
        for (int i = 0; i < bufferSize; i++){
                left[i]         = input[i*2]*0.5;
                right[i]        = input[i*2+1]*0.5;
                curVol += left[i] * left[i];
                curVol += right[i] * right[i];
                numCounted+=2;
        }
        curVol /= (float)numCounted;
        curVol = sqrt( curVol );
        smoothedVol *= 0.93;
        smoothedVol += 0.07 * curVol;
        bufferCounter++;
}

void testApp::setup() {
	 width=ofGetScreenWidth();
         height=ofGetScreenHeight();
	 xr = 0;
	 yr = ofRandom(0,TWO_PI);
         stages=2;
	 ofEnableDepthTest();
	 ofDisableArbTex();
         matCap.loadImage("matcap4.jpg");
         shader.load("sem");

         ofSetSmoothLighting(true);
         light.setDiffuseColor( ofFloatColor(85, 245, 55) );
         light.setSpecularColor( ofFloatColor(1, 133, 234));
         material.setShininess( 120 );
         material.setSpecularColor(ofColor(5, 5, 255, 55));
         light.setPosition(200,200,200);

	 /* audio in */
         soundStream.listDevices();
         int bufferSize = 256;
         left.assign(bufferSize, 0.0);
         right.assign(bufferSize, 0.0);
         volHistory.assign(400, 0.0);
         bufferCounter   = 0;
         drawCounter     = 0;
         smoothedVol     = 0.0;
         scaledVol       = 0.0;
         soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	 ofxTimeline::removeCocoaMenusFromGlut("Camera Track");

         timeline.setup();
         timeline.setLoopType(OF_LOOP_NORMAL);
         timeline.setDurationInSeconds(30);
	 cameraTrack = new ofxTLCameraTrack();
         cameraTrack->setCamera(cam);
         timeline.addTrack("Camera", cameraTrack);
         timeline.addCurves("Rotate X", ofRange(0, 360));
         timeline.addCurves("Rotate Y", ofRange(0, 360));

         cameraTrack->lockCameraToTrack = true;
         timeline.play();
}

void testApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        volHistory.push_back( scaledVol );
        if( volHistory.size() >= 400 ){
                volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }
}

void testApp::draw() {
   ofBackgroundGradient(ofColor(0),ofColor(0),OF_GRADIENT_CIRCULAR);
   
   ofPushMatrix();
 //  ofRotate(timeline.getValue("Rotate X"), 1, 0, 0);
 //  ofRotate(timeline.getValue("Rotate Y"), 0, 1, 0);

   cam.begin();
   // ofTranslate(width/2, height/2, -240);
   
   if(shaderoff){
        ofEnableDepthTest();
   	shader.begin();
	shader.setUniformTexture("tMatCap", matCap, 1);
   }else{
	ofEnableDepthTest();
        ofEnableLighting();
        light.enable();
        material.begin();
   }
   cube(20,20,20,700,stages,ofColor(255,255,255));
   if(shaderoff) 
      shader.end();
   else{
      material.end();
      ofDisableLighting();
      ofDisableDepthTest();
   }
   cam.end();
   ofPopMatrix();
   
   if(drawTimeLine)
      timeline.draw();
}

void testApp::keyPressed(int key){
	if(key=='1')
	   matCap.loadImage("matcap1.jpg");
	if(key=='2')
	   matCap.loadImage("matcap2.jpg");
	if(key=='3')
	   matCap.loadImage("matcap3.jpg");
	if(key=='4')
	   matCap.loadImage("matcap4.jpg");
	if(key=='5')
	   matCap.loadImage("matcap5.jpg");
	if(key=='6')
	   matCap.loadImage("matcap6.jpg");
	if(key=='7')
	   matCap.loadImage("matcap7.jpg");
	if(key=='8')
	   matCap.loadImage("matcap8.jpg");
	if(key=='9')
	   matCap.loadImage("matcap9.jpg");
	if(key=='0')
	   matCap.loadImage("matcap.jpg");
	if(key=='q')
	   shaderoff=!shaderoff;

	if(key=='f')
	   ofToggleFullscreen();
        if(key=='d')
	   drawTimeLine=!drawTimeLine;
	if(key=='s')
	   ofSaveFrame();
        if(key=='l')
           cameraTrack->lockCameraToTrack = !cameraTrack->lockCameraToTrack;
        if(key == 't')
           cameraTrack->addKeyframe();
}

void testApp::cube(float x, float y, float z, float s, int n, ofColor colors) {
  if(n>0) {
     n--;
     float u = s/3;
     for(float i=-u; i<=u; i+=u)
        for(float j=-u; j<=u; j+=u)
           for(float k=-u; k<=u; k+=u)
             if(abs(i) + abs(j) + abs(k) >= 2*u){
               cube(x+i, y+j, z+k, u, n, colors);
               }
   }
   else {
     ofPushMatrix();
     ofTranslate(x, y, z);
     ofFill();
     ofSetColor(colors);
     ofDrawBox(s);
     ofNoFill();
     ofSetColor(0);
     ofDrawBox(s);
     ofPopMatrix();
  }
}
