#include "Cena.h"

CenaUniverso::CenaUniverso( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/filterTexture.c");

    fboUniverso.allocate(1024, 768);
    videoUniverso.load("../data/universo.mp4");
    videoUniverso.play();
}

void CenaUniverso::update( float dt ) {

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaUniverso::filtraImg() {
    ofSetColor(255,255,255);
    videoUniverso.update();
    fboUniverso.begin();
    ofClear(0,0,0, 0);
    videoUniverso.draw(0,0,1024,768);
    fboUniverso.end();

    ofSetColor(255,255,255);
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255);
    shaderCena.begin();
    shaderCena.setUniformTexture("texture1", fboUniverso.getTextureReference(), 1);
    ofSetColor(255,255,255);
    ku->drawImg();
    shaderCena.end();

    fboCena.end();
}

void CenaUniverso::drawAtivo() {
    ofSetColor(255,255,255);
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

    ImGui::End();
}
