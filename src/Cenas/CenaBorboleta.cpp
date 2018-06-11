#include "Cena.h"

CenaBorboleta::CenaBorboleta( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 200;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/greenAsTransparent.c");

    borboleta.load("../data/mariposa.mp4");
    fundo.load("../data/fundo.mp4");
    borboleta.play();
    fundo.play();
    posBorboleta.set(-200,400);
}

void CenaBorboleta::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        posBorboleta.x = ofLerp(posBorboleta.x, ku->centroMassa.x*1024, 0.01);
        posBorboleta.y = ofLerp(posBorboleta.y, ku->centroMassa.y*768, 0.1) + (ofNoise(dt)-0.5)*100;

        filtraImg();
    }
}

void CenaBorboleta::filtraImg() {
    fundo.update();
    borboleta.update();
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    fundo.draw(0,0,1024,768);
    shaderCena.begin();
    //cout << "centro massa: [" << ku->centroMassa.x << "," << ku->centroMassa.y << "]";
    imgBorboleta.setFromPixels( borboleta.getPixels());
    if( posBorboleta.x > ku->centroMassa.x*1024 ) {
        imgBorboleta.mirror(false,true);
    }
    imgBorboleta.draw( posBorboleta.x - 128 , posBorboleta.y - 80 , 256,191);
    ofSetColor(255,255,255);
    shaderCena.end();
    fboCena.end();
}

void CenaBorboleta::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaBorboleta::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaBorboleta::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Borboleta");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);

    ImGui::End();
}
