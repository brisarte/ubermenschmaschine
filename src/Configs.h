#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class Configs: public ofBaseApp {
    public: 
        void setup();
        void update();
        void draw();
        void mousePressed(int x, int y, int iButton) ;

        ofxImGui::Gui configs;

        ofxKinect kinectGlobal;
        int anguloKinect;
        void ligaKinect();
        void desligaKinect();
        ofxCvGrayscaleImage depthCam;
        ofPixels depthPixels;
        float depthMedia;
        ofVec2f centroMassa;
        void calculaMassa();

};
