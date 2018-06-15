#include "Cena.h"

CenaNascendo::CenaNascendo( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 21;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/greenAsTransparent.c");

    video.load("../data/nascendo.mp4");
    video.play();
    video.setPaused(true);
}

void CenaNascendo::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        video.setPaused(false);
        filtraImg();
    } else {
        video.setPaused(true);
    }
}

void CenaNascendo::filtraImg() {
    video.update();
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    video.draw(0,0,1024,768);
    fboCena.end();
}

void CenaNascendo::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaNascendo::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaNascendo::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Nascendo");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);

    ImGui::End();
}

void CenaNascendo::limpaCena() {
    video.setPosition(0);
}
