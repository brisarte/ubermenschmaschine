#pragma once

#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class KinectUtils {

    public:
        // Funções default
        void setup(), update(), draw();

        ofxKinect kinectGlobal;
        int anguloKinect, contrasteKinect, brilhoKinect;
        // Funções de controle liga/desliga
        void ligaKinect();
        void desligaKinect();
        ofxCvGrayscaleImage depthCam;
        // Variáveis de informação sobre a profundidade
        ofPixels depthPixels;
        float depthAvg;
        ofVec2f centroMassa;
        void calculaMassa();

        // Interface Simples
        void drawGUI();
        void drawMiniatura(int x = 5,
                           int y = 5,
                           int w = 320,
                           int h = 240);
};
