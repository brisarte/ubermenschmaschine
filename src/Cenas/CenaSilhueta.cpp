#include "Cena.h"

CenaSilhueta::CenaSilhueta( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 20;
    tempoTransicao = 0.1;
    qtdBlur = 37;
    qtdRastro = 80;
    shaderCena.load("../data/vertexdummy.c","../data/blackAsAlpha.c");
}

void CenaSilhueta::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
    }
}

void CenaSilhueta::filtraImg() {
    ku->setErodeAndDilate(true);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,220,205);
    ku->drawImg();
    ofSetColor(30,200,200);
    ku->drawImg(true);
    fboCena.end();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

void CenaSilhueta::drawAtivo() {
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
    ImGui::SliderInt("blur", &qtdBlur, 0, 120);
    ImGui::SliderInt("rastro", &qtdRastro, 0, 100);

    ImGui::End();
}
