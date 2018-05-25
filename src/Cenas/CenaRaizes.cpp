#include "Cena.h"

CenaRaizes::CenaRaizes( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 30;
    tempoTransicao = 3;
    shaderCena.load("../data/vertexdummy.c","../data/blackAsTransparent.c");
    shaderFlor.load("../data/vertexdummy.c","../data/greenAsTransparent.c");

    musicaCena.load("../data/raizes.mp3");
}

void CenaRaizes::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

        for( int i = 0; i < videosRaiz.size(); i++ ) {
            if( videosRaiz[i]->video.isLoaded() && active )  {
                videosRaiz[i]->video.update();
            }
        }
        for( int i = 0; i < videosFlor.size(); i++ ) {
            if( videosFlor[i]->video.isLoaded() && active )  {
                videosFlor[i]->video.update();
            }
        }

        for(int i = 0; i < 20; i++) {
            if( active && timeElapsed > i*2 && videosRaiz.size() < i+1 ) {
                adicionaRaiz(i);
            }
            if( active && timeElapsed > i + 10 && videosFlor.size() < i+1 ) {
                adicionaFlor(i);
            }
        }
        filtraImg();
    }
}

void CenaRaizes::adicionaRaiz(int i) {
    VideoRaiz* videoRaiz = new VideoRaiz();
    videoRaiz->video.loadAsync(std::string("../data/raiz") + std::to_string((int)ofRandom(4)) + std::string(".mp4") );
    videoRaiz->video.setLoopState(OF_LOOP_NONE);
    videoRaiz->video.play();
    videoRaiz->inicio.set( ofRandom( -5*i, 5*i), ofRandom(200-20*i, 100-25*i) );
    videoRaiz->rotacao =  ofRandom( -40, 40);
    videosRaiz.push_back(videoRaiz);
}

void CenaRaizes::adicionaFlor(int i) {
    VideoRaiz* videoFlor = new VideoRaiz();
    videoFlor->video.loadAsync(std::string("../data/flor") + std::to_string((int)ofRandom(2)) + std::string(".mp4") );
    videoFlor->video.setLoopState(OF_LOOP_NONE);
    videoFlor->video.play();
    videoFlor->inicio.set( ofRandom( -10*i-100, 10*i+100), ofRandom(-10*i, -26*i) );
    videoFlor->rotacao =  ofRandom( -40, 40);
    cout << "adiconando flor " << i << " videoLoaded:" << videoFlor->video.isLoaded() << "\n";
    videosFlor.push_back(videoFlor);
}

void CenaRaizes::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);\ 
    ofSetColor(5,20,50);
    ku->depthCam.blurHeavily();
    ku->depthCam.draw(0,0,1024,768);
    shaderCena.begin();
    ofSetColor(255,255,255);
    //Desenha Raizes
    for( int i = videosRaiz.size()-1; i >= 0; i-- )
    {
        if( videosRaiz[i]->video.isLoaded() && active )  {
            ofPushMatrix();        // push the current coordinate position
            ofTranslate(videosRaiz[i]->inicio.x+ku->centroMassa.x * 1024, videosRaiz[i]->inicio.y+768);

            ofRotateZ(videosRaiz[i]->rotacao);
            cout << "desenhando raiz " << i << " [" << videosRaiz[i]->inicio.x+ku->centroMassa.x * 1024 << "," << videosRaiz[i]->inicio.y+768 << "]\n";
            videosRaiz[i]->video.draw(-512,-768, 1024, 768);
            ofPopMatrix();
        }
    }
    shaderCena.end();
    shaderFlor.begin();
    //Desenha Flores
    for( int i = videosFlor.size()-1; i >= 0; i-- )
    {
        if( videosFlor[i]->video.isLoaded() && active )  {
            ofPushMatrix();        // push the current coordinate position
            ofTranslate(videosFlor[i]->inicio.x+ku->centroMassa.x * 1024, videosFlor[i]->inicio.y+768);
            cout << "desenhando flor " << i << " [" << videosFlor[i]->inicio.x+ku->centroMassa.x * 1024 << "," << videosFlor[i]->inicio.y+768 << "]\n";
            ofRotateZ(videosFlor[i]->rotacao);
            videosFlor[i]->video.draw(-51,-77, 102, 76);
            ofPopMatrix();
        }
    }
    shaderFlor.end();
    fboCena.end();
}

void CenaRaizes::drawAtivo() {
    ofSetColor(255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaRaizes::drawTransicao() {
    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/tempoTransicao;
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);
}

void CenaRaizes::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Raízes");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);
    ImGui::SliderInt("blur", &qtdBlur, 0, 100);
    ImGui::SliderInt("rastro", &qtdRastro, 0, 100);

    ImGui::End();
}
