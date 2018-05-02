#include "Cena.h"

CenaRaizes::CenaRaizes( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 5;
    tempoTransicao = 3;
    shaderCena.load("../data/vertexdummy.c","../data/blackAsAlpha.c");
}

void CenaRaizes::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);

		for( int i = 0; i < videosRaiz.size(); i++ ) {
        	if( videosRaiz[i]->video.isLoaded() && active )  {
            	videosRaiz[i]->video.update();
        	}
		}

		for(int i = 0; i < 10; i++) {
				if( active && timeElapsed > i && videosRaiz.size() < i+1 ) {
						adicionaRaiz();
				}
		}
        filtraImg();
    }
}

void CenaRaizes::adicionaRaiz() {
		VideoRaiz* videoRaiz = new VideoRaiz();
		videoRaiz->video.load("../data/raiz.mp4");
		videoRaiz->video.play();
		videoRaiz->inicio.set( ofRandom( -100, 100), 0 );
		videosRaiz.push_back(videoRaiz);
}

void CenaRaizes::filtraImg() {
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofSetColor(5,20,50);
    ku->depthCam.blurHeavily();
    ku->depthCam.draw(0,0,1024,768);
    shaderCena.begin();
    ofSetColor(255,255,255);
	//Desenha Raizes
	for( int i = videosRaiz.size()-1; i >= 0; i-- )
	{
		if( videosRaiz[i]->video.isLoaded() && active )  {
			videosRaiz[i]->video.draw(ku->centroMassa.x - 422 + videosRaiz[i]->inicio.x, i*(-50) + videosRaiz[i]->inicio.y, 1024, 771);
		}
	}
    shaderCena.end();
    ofSetColor(30,55,150);
    ofDrawCircle( ku->centroMassa.x/640 * 1024, ku->centroMassa.y/480 * 768, 6);
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

    ImGui::End();
}
