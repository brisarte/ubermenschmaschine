#include "Cena.h"

CenaEntrada::CenaEntrada( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 10;
    tempoTransicao = 1;
    massaMaxima = 50000;
    shaderCena.load("../data/vertexdummy.c","../data/silhuetaInvertida.c");
    shaderFiltraImg.load("../data/vertexdummy.c","../data/filterTexture.c");
    imgCarne.load("../data/carne.png");
    fboCarne.allocate(1024, 768);
    nivelProporcao = 3;
}

void CenaEntrada::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        filtraImg();
        if(ku->depthTotal > massaMaxima) {
            desligaCena(true);
        }
    }
}

void CenaEntrada::filtraImg() {
    ofSetColor(255,255,255);
    fboCarne.begin();
    ofClear(0,0,0, 0);
    imgCarne.draw(0,0,1024,768);
    fboCarne.end();

    ofSetColor(255,255,255);
    fboCena.begin();
    ofClear(0,0,0 ,0);
    ofBackground(0,0,0);
  
    float nDepth = ku->depthTotal;
    if(nDepth == 0) nDepth = 0.01;
    float proporcao = 1-(nDepth/massaMaxima);
    proporcao *= nivelProporcao;

    // Desenha imagem com textura
    shaderFiltraImg.begin();
    shaderFiltraImg.setUniformTexture("texture1", fboCarne.getTextureReference(), 1);
    ku->depthCam.draw(ku->centroMassa.x-(512*proporcao),768*(1-proporcao),1024*proporcao,768*proporcao);
    shaderFiltraImg.end();

    // Desenha imagem menor
    shaderCena.begin();
    ku->depthCam.draw(ku->centroMassa.x-512,0,1024,768);
    shaderCena.end();
    
    fboCena.end();
}

void CenaEntrada::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaEntrada::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaEntrada::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Entrada");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);
    ImGui::SliderInt("massa máxima", &massaMaxima, 0, 150000);
    ImGui::SliderFloat("nível proporção", &nivelProporcao, 0, 5);

    ImGui::End();
}
