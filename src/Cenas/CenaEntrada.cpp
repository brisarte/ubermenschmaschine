#include "Cena.h"

CenaEntrada::CenaEntrada( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 80;
    tempoTransicao = 0.2;
    qtdBlur = 0;
    qtdRastro = 0;

    massaMaxima = 85000;
    shaderCena.load("../data/vertexdummy.c","../data/silhuetaInvertida.c");
    shaderFiltraImg.load("../data/vertexdummy.c","../data/filterTexture.c");
    imgCarne.load("../data/carne.png");

    fboCarne.allocate(1024, 768);
    nivelProporcao = 1;
    musicaCena.load("../data/entrada.mp3");
    musicaCena.play();
    musicaCena.setPaused(true);
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
    float proporcao = 2-(nDepth/massaMaxima);
    proporcao *= nivelProporcao;

    // Desenha imagem com textura
    shaderFiltraImg.begin();
    shaderFiltraImg.setUniformTexture("texture1", fboCarne.getTextureReference(), 1);
    ku->depthCam.draw( (1-proporcao)*512 + (proporcao*512) - (proporcao*ku->centroMassa.x*1024),768*(1-proporcao),1024*proporcao,768*proporcao);
    shaderFiltraImg.end();

    // Desenha imagem menor
    shaderCena.begin();
    ku->depthCam.draw( 512 - ku->centroMassa.x*1024 ,0,1024,768);
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
    ImGui::SliderInt("blur", &qtdBlur, 0, 100);
    ImGui::SliderInt("rastro", &qtdRastro, 0, 100);

    ImGui::SliderFloat("proporção", &nivelProporcao, 0, 5);
    ImGui::SliderInt("massa máxima", &massaMaxima, 0, 150000);

    ImGui::End();
}
