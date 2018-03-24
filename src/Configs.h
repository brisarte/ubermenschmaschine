#pragma once

#include "ofxImGui.h"
#include "ofMain.h"

#include "KinectUtils.h"

class Configs: public ofBaseApp {
    public: 
        void setup();
        void update();
        void draw();
        void mousePressed(int x, int y, int iButton) ;

        ofxImGui::Gui configs;

        KinectUtils ku;
};
