#include "Cena.h"

CenaAgua::CenaAgua( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 3;
    shaderCena.load("../data/vertexdummy.c","../data/aguaKinect.c");
    imgAgua.load("../data/agua.jpg");
}

void CenaAgua::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaAgua::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(5,120,180);
    shaderCena.begin();
    ku->depthCam.blur(9);
    shaderCena.setUniformTexture( "agua",imgAgua.getTextureReference(), 1);
    ku->depthCam.draw(0,0,1024,768);
    shaderCena.end();
    fboCena.end();
}

void CenaAgua::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaAgua::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

