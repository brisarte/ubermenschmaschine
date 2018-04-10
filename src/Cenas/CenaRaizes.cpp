#include "Cena.h"

CenaRaizes::CenaRaizes( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 3;
    videoRaiz.load("../data/raiz.mp4");
    shaderCena.load("../data/vertexdummy.c","../data/blackAsAlpha.c");
    videoRaiz.play();
}

void CenaRaizes::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        if( videoRaiz.isLoaded() )  {
            videoRaiz.update();
        }

        filtraImg();
    }
}

void CenaRaizes::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(5,20,50);
    ku->depthCam.blurHeavily();
    ku->depthCam.draw(0,0,1024,768);
    shaderCena.begin();
    ofSetColor(255,255,255);
    if( videoRaiz.isLoaded() )  {
        videoRaiz.draw(ku->centroMassa.x - 422, 0, 1024, 771);
        videoRaiz.draw(ku->centroMassa.x - 622, 0, 1024, 771);
        videoRaiz.draw(ku->centroMassa.x - 522, 0, 1024, 771);
        videoRaiz.draw(ku->centroMassa.x - 582, 0, 1024, 771);
    }
    shaderCena.end();
    ofSetColor(30,55,150);
    ofDrawCircle( ku->centroMassa.x/640 * 1024, ku->centroMassa.y/480 * 768, 6);
    fboCena.end();
}

void CenaRaizes::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaRaizes::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

