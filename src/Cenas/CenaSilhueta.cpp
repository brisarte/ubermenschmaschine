#include "Cena.h"

CenaSilhueta::CenaSilhueta( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/blackAsAlpha.c");
}

void CenaSilhueta::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaSilhueta::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(150,220,0);
    ku->depthCam.draw(0,0,1024,768);
    shaderCena.begin();
    ofSetColor(255,255,255);
    shaderCena.end();
    fboCena.end();
}

void CenaSilhueta::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaSilhueta::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaSilhueta::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Silhueta");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);

    ImGui::End();
}
