#include "Cena.h"

Borboleta::Borboleta(float x, float y) {
    posBorboleta.set(x,y);

    video.load("../data/mariposa.mp4");
    video.play();
    video.setPosition(ofRandom(0,1));
    video.setPaused(true);
}

CenaBorboleta::CenaBorboleta( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 138;
    tempoTransicao = 0.1;
    shaderCena.load("../data/vertexdummy.c","../data/greenAsTransparent.c");

    fboFundo.allocate(1024,768);
    shaderSilhueta.load("../data/vertexdummy.c","../data/silhueta.c");
    fundo.load("../data/fundo.mp4");
    fundo.play();
    criaBorboletas();
}

void CenaBorboleta::criaBorboletas() {
    Borboleta* borboleta1 = new Borboleta(-200, 400);
    Borboleta* borboleta2 = new Borboleta(1300, 400);
    Borboleta* borboleta3 = new Borboleta(1300, -200);
    Borboleta* borboleta4 = new Borboleta(512, -500);
    Borboleta* borboleta5 = new Borboleta(800, -500);
    Borboleta* borboleta6 = new Borboleta(2000, 500);
    Borboleta* borboleta7 = new Borboleta(-1000, 500);
    Borboleta* borboleta8 = new Borboleta(-500, 500);
    panapana.push_back(borboleta1);
    panapana.push_back(borboleta2);
    panapana.push_back(borboleta3);
    panapana.push_back(borboleta4);
    panapana.push_back(borboleta5);
    panapana.push_back(borboleta6);
    panapana.push_back(borboleta7);
    panapana.push_back(borboleta8);
}

void CenaBorboleta::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        for( int i = 0; i < panapana.size(); i++ ) {
            panapana[i]->video.setPaused(false);
            panapana[i]->posBorboleta.x = ofLerp(panapana[i]->posBorboleta.x, ku->centroMassa.x*1024 + (ofNoise((timeElapsed*i)/3)-0.5)*2000, 0.05);
            panapana[i]->posBorboleta.y = ofLerp(panapana[i]->posBorboleta.y, 200 - (ofNoise(timeElapsed/(i+1))-0.5)*800 , 0.1);
        }

        //Causa caos no meio do video
        if( timeElapsed > 30 ) {
            int baguncaBorboleta = timeElapsed*3-90;
            for( int i = 0; i < panapana.size(); i++ ) {
                panapana[i]->posBorboleta.x = ofLerp(panapana[i]->posBorboleta.x, ku->centroMassa.x*1024, 0.01) + ofRandom(-baguncaBorboleta,baguncaBorboleta);
                panapana[i]->posBorboleta.y = ofLerp(panapana[i]->posBorboleta.y, ku->centroMassa.y*768, 0.1) + ofRandom(-baguncaBorboleta,baguncaBorboleta);
            }
        }

        fundo.setPaused(false);
        filtraImg();
    } else {
        for( int i = 0; i < panapana.size(); i++ ) {
            panapana[i]->video.setPaused(true);
        }
        fundo.setPaused(true);
    }
}

void CenaBorboleta::filtraImg() {
    fundo.update();
    for( int i = 0; i < panapana.size(); i++ ) {
        panapana[i]->video.update();
    }
    fboFundo.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    fundo.draw(0,0,1024,768);
    fboFundo.end();

    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(255,255,255, 255);
    
    shaderSilhueta.begin();
    shaderSilhueta.setUniformTexture("texture1", fboFundo.getTextureReference(), 1);
    shaderSilhueta.setUniformTexture("kinecto", ku->fboImgCam.getTextureReference(), 2);
    ku->drawImg();
    shaderSilhueta.end();
    
    shaderCena.begin();
    ofSetColor(255,255,255);
    for( int i = 0; i < panapana.size(); i++ ) {
        panapana[i]->imgBorboleta.setFromPixels( panapana[i]->video.getPixels());
        if( panapana[i]->posBorboleta.x > ku->centroMassa.x*1024 ) {
            panapana[i]->imgBorboleta.mirror(false,true);
        }
        if(timeElapsed > 50 && timeElapsed < 53 ) {
            panapana[i]->imgBorboleta.mirror(true, false);
        }
        if(timeElapsed < 100) {
            panapana[i]->imgBorboleta.draw( panapana[i]->posBorboleta.x - 178 , panapana[i]->posBorboleta.y - 128 , 341,256);
        }
    }
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

void CenaBorboleta::limpaCena() {
    fundo.setPosition(0);
}
