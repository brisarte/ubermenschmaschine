#include "Cena.h"

CenaEntrada::CenaEntrada( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 3;
    shaderCena.load("../data/vertexdummy.c","../data/silhuetaInvertida.c");
}

void CenaEntrada::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaEntrada::filtraImg() {
    fboCena.begin();
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    // Normaliza depthAvg
    float nDepth = ku->depthAvg/255;
    if(nDepth == 0) nDepth = 0.01;
    float proporcao = 1/nDepth;
    ku->depthCam.draw(ku->centroMassa.x,0,1024*proporcao,768*proporcao);
    shaderCena.begin();
    ku->depthCam.draw(ku->centroMassa.x,384,512,384);
    shaderCena.end();
    fboCena.end();
}

void CenaEntrada::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaEntrada::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

