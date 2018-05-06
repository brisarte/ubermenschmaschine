#include "Cena.h"

CenaAgua::CenaAgua( KinectUtils *kutils, bool ativo ) {
	kutils->rastro = 20;
    setup(kutils, ativo);
    tempoMaximo = 20;
    tempoTransicao = 1;
    qtdBlur = 15;
    shaderCena.load("../data/vertexdummy.c","../data/aguaShader.c");
}

void CenaAgua::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        ku->blur = qtdBlur;
        filtraImg();
    }
}

void CenaAgua::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255);
    shaderCena.begin();
    shaderCena.setUniform1f( "iTime", timeElapsed );
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

void CenaAgua::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Água");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);
    ImGui::SliderInt("blur", &qtdBlur, 0, 120);

    ImGui::End();
}
