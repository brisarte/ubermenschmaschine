#include "Cena.h"

CenaCorpos::CenaCorpos( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 83;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/greenAsTransparent.c");

    video.load("../data/corpos.mp4");
    video.play();
    video.setPaused(true);
    musicaCena.load("../data/corpos.mp3");
}

void CenaCorpos::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        video.setPaused(false);
        filtraImg();
    } else {
        video.setPaused(true);
    }
}

void CenaCorpos::filtraImg() {
    video.update();
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    video.draw(ku->centroMassa.x*1024-512,0,1024,768);
    fboCena.end();
}

void CenaCorpos::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaCorpos::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaCorpos::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Corpos");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);

    ImGui::End();
}

void CenaCorpos::limpaCena() {
    video.setPosition(0);
}
