#include "Cena.h"

CenaUniverso::CenaUniverso( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 22;
    tempoTransicao = 3;
    qtdBlur = 0;
    qtdRastro = 0;
    shaderCena.load("../data/vertexdummy.c","../data/filterTexture.c");

    fboUniverso.allocate(1024, 768);
    videoUniverso.load("../data/video/universo.mp4");
    videoUniverso.play();
    musicaCena.load("../data/audio/universo.mp3");
}

void CenaUniverso::update( float dt ) {

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaUniverso::filtraImg() {
    qtdBlur = timeElapsed*4+1;
    //ku->brilhoKinect = timeElapsed*6;
    for(int i = 0; i < timeElapsed*8; i++) {
        ku->depthCam.dilate();
    }
    ofSetColor(255,255,255, 255);
    videoUniverso.update();
    fboUniverso.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    videoUniverso.draw(0,0,1024,768);
    fboUniverso.end();

    ofSetColor(255,255,255,255);
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255,255);
    shaderCena.begin();
    shaderCena.setUniformTexture("texture1", fboUniverso.getTextureReference(), 1);
    ofSetColor(255,255,255,255);
    ku->drawImg();
    shaderCena.end();

    fboCena.end();
}

void CenaUniverso::drawAtivo() {
    ofSetColor(255,255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaUniverso::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaUniverso::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Universo");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);
    ImGui::SliderInt("blur", &qtdBlur, 0, 100);
    ImGui::SliderInt("rastro", &qtdRastro, 0, 100);

    ImGui::End();
}

void CenaUniverso::limpaCena() {
    videoUniverso.setPosition(0);
}
