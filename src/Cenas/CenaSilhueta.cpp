#include "Cena.h"

Particula::Particula(float x, float y) {
    // ajusta de 640:480 para 1024:768
    pos.set( (x/640)*1024, (y/480)*768);
    velocidade.set( ofRandom(-20, 20), ofRandom(0, 50) );
    cor.set(255,220,255,255);
    cor.setBrightness(100);
}

void Particula::update(float dt, ofVec2f aceleracao) {
    velocidade+= aceleracao * dt;
    pos += velocidade * dt;
    if ( pos.y > 768 ) {
        velocidade.set( ofRandom(-20, 20) , -velocidade.y/2 );
    }
    //cor.setHue( cor.getHue() + dt*100 );
    cor.setBrightness( cor.getBrightness() - dt*15 );
}

void Particula::draw() {
    ofSetColor(cor);
    ofDrawCircle( pos, 1);
}

CenaSilhueta::CenaSilhueta( KinectUtils *kutils, bool ativo ) {
    setup(kutils, ativo);
    tempoMaximo = 20;
    tempoTransicao = 4;
    qtdBlur = 37;
    qtdRastro = 80;
    gravidade = 200;
    posEgo.set(10,300);
    aceleracaoEgo.set(10,0);
}

void CenaSilhueta::update( float dt ) {
    if( active ) {
        atualizaTransicoes(dt);
        // calcula posicao do ego
        float distanciaEgo = (posEgo.x - ku->centroMassa.x*1024);
        if(distanciaEgo > 0) {
            aceleracaoEgo.x = 1024 - distanciaEgo;
            aceleracaoEgo.x = aceleracaoEgo.x * aceleracaoEgo.x * aceleracaoEgo.x;
            aceleracaoEgo.x /= 1000000;
        } else {
            aceleracaoEgo.x = -1024 - distanciaEgo;
            aceleracaoEgo.x = aceleracaoEgo.x * aceleracaoEgo.x * aceleracaoEgo.x;
            aceleracaoEgo.x /= 1000000;
        }


        // Caso esteja acabando, ignora o limite e atrai a silhueta
        if( timeElapsed < (tempoMaximo - 2) ) {
            int espacoLimite = timeElapsed*4 + 100;

            posEgo.x += aceleracaoEgo.x/100 * (sin(timeElapsed*4)+0.8);
            // Faz a sombra ~dançar
            posEgo.x += (ofNoise(timeElapsed*5)-0.5)*10;
            posEgo.x += (ofNoise(timeElapsed*3)-0.5)*12;
            if(posEgo.x < espacoLimite) {
                posEgo.x = espacoLimite;
            }
            if(posEgo.x > 1024 - espacoLimite) {
                posEgo.x = 1024 - espacoLimite;
            }
        } else {
            posEgo.x = ofLerp( posEgo.x, ku->centroMassa.x*1024, 0.02);
            if(abs(distanciaEgo) < 20) {
                posEgo.x = ofLerp( posEgo.x, ku->centroMassa.x*1024, 0.02);
            }
            if(tempoMaximo < timeElapsed) {
                setTransicao(true);
            }
        }
        // Caso o espaco esteja limitado, desliga a cena
        /*
        if(espacoLimite > 502 && abs(distanciaEgo) < 2) {
            setTransicao(true);
        }
        */

        filtraImg();
        if (timeElapsed > timeStartTransicao) {
            ofVec2f aceleracao(0, gravidade);
            for( int i = 0; i < particulas.size(); i++ ) {
                particulas[i]->update(dt, aceleracao);
            }
        }
    }
}

void CenaSilhueta::criaParticulasKinect(ofPixels depthPixels) {
    int i = 0;
    while( i < depthPixels.size() ) {
        int brilho = depthPixels[i];
        if( brilho > 0 && ofRandom(0, 1000) > 980 ) {
            int x = i % 640;
            int y = floor(i/640);
            particulas.push_back( new Particula(x, y) ); 
        }
        i++;
    }
}

void CenaSilhueta::filtraImg() {
    ku->setErodeAndDilate(false);
    ofSetColor(255,255,255, 255);
    fboCena.begin();
    ofClear(0,0,0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255,220,205);
    ku->depthCam.draw( (posEgo.x - ku->centroMassa.x*1024) ,0,1024,768);
    ofSetColor(30,200,200);
    ku->depthCam.draw( 0 ,0,1024,768);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofDrawCircle(posEgo, 10);
    fboCena.end();
}

void CenaSilhueta::drawAtivo() {
    ofSetColor(255,255,255,255);
    fboCena.draw(0,0,1024,768);
}

void CenaSilhueta::drawTransicao() {
    qtdBlur = 0;
    qtdRastro = 0;

    // Numero normalizado de acordo com o tempo de transição
    float nFade = 1 - (timeElapsed - timeStartTransicao)/(tempoTransicao/15);
    ofSetColor( nFade*255, nFade*255, nFade*255 );
    fboCena.draw(0,0,1024,768);

    if (ofRandom( 0, (1 - (timeElapsed - timeStartTransicao)/(tempoTransicao/5))*20 ) > 0)
        criaParticulasKinect(ku->depthPixels); 

    for( int i = 0; i < particulas.size(); i++ ) {
        particulas[i]->draw();
    }
}

void CenaSilhueta::drawConfigs() {
    // Inicia a janela de configs
    ImGui::SetNextWindowSize(ofVec2f(330, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Silhueta");

    ImGui::SliderFloat("duração", &tempoMaximo, 0, 120);
    ImGui::SliderInt("blur", &qtdBlur, 0, 120);
    ImGui::SliderInt("rastro", &qtdRastro, 0, 100);

    ImGui::SliderInt("gravidade", &gravidade, -100, 700);

    ImGui::End();
}
