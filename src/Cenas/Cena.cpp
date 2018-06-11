#include "Cena.h"

void Cena::setup( KinectUtils *kutils, bool ativo) {
    ku = kutils;

    fboCena.allocate(1024,768);
    setAtivo(ativo);
    inTransition = false;
    timeElapsed = 0;
    qtdBlur = 0;
    qtdRastro = 0;
}

void Cena::atualizaTransicoes( float dt ) {
    timeElapsed += dt;
    ku->blur = qtdBlur;
    ku->rastro = qtdRastro;
    ku->setErodeAndDilate();
    // Ativa transição entre cenas
    if( timeElapsed > tempoMaximo && inTransition == false) {
        setTransicao(true);
    }
    // Desativa cena 
    if( timeElapsed > timeStartTransicao + tempoTransicao && inTransition == true) {
        active = false;
    }
}

void Cena::update( float dt ) {}

void Cena::drawConfigs() {}

void Cena::draw() {
    if(active) {
        if(!inTransition) {
            drawAtivo();
        } else {
            drawTransicao();
        }
    }
}

void Cena::drawAtivo() {}

void Cena::drawTransicao() {}

void Cena::setTransicao( bool tr ) {
    if ( tr && !inTransition ) {
        // Caso esteja desativando, anota o horário
        timeStartTransicao = timeElapsed;
        // E força o tempoMaximo para agora
        tempoMaximo = timeElapsed;
    }
    inTransition = tr;
}

void Cena::setAtivo( bool at ) {
    active = at;
    if( active ) {
        if( musicaCena.isLoaded() ) {
            musicaCena.play();
        }
    } else {
        if( musicaCena.isLoaded() ) {
            musicaCena.stop();
        }
    }
}

void Cena::resetTimer() {
    timeElapsed = 0;
}


void Cena::desligaCena( bool transition ) {
    setAtivo(false);
    inTransition = transition;
    resetTimer();
}

