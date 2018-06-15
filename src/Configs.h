#pragma once

#include "ofxImGui.h"
#include "ofMain.h"

#include "KinectUtils.h"
#include "Cena.h"

class Configs: public ofBaseApp {
    public: 
        void setup();
        void update();
        void draw();
        void mousePressed(int x, int y, int iButton) ;

        ofxImGui::Gui configs;

        KinectUtils ku;

        vector<Cena*>   cenas;
        CenaEntrada*    cenaEntrada; 
        CenaAgua*       cenaAgua;
        CenaRaizes*     cenaRaizes; 
        CenaBorboleta*  cenaBorboleta;
        CenaSol*        cenaSol;
        CenaSilhueta*   cenaSilhueta;
        CenaUniverso*   cenaUniverso;
        CenaNascendo*   cenaNascendo;
        CenaAbismo*     cenaAbismo;
        CenaCorpos*     cenaCorpos;

        float t0, t1, dt;

        int cenaAtual;
        void resetMoksha();
        void proximaCena();
        void voltaCena();
        void resetCena(int i);
};
