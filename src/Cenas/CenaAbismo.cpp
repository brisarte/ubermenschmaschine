#include "Cena.h"

CenaAbismo::CenaAbismo( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 42;
    tempoTransicao = 0.1;

    video.load("../data/abismo.mp4");
    video.play();
    video.setPaused(true);
}

void CenaAbismo::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        video.setPaused(false);
        filtraImg();
    } else {
        video.setPaused(true);
    }
}

void CenaAbismo::filtraImg() {
    video.update();
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    video.draw(ku->centroMassa.x*256-128,0,1024,768);
    fboCena.end();
}

void CenaAbismo::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaAbismo::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaAbismo::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Abismo");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);

    ImGui::End();
}

void CenaAbismo::limpaCena() {
    video.setPosition(0);
}
