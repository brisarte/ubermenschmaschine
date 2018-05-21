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
    // Caso esteja desativando, anota o horário
    if ( tr ) {
        timeStartTransicao = timeElapsed;
        cout << "\t startTransicao:" << timeStartTransicao;
    }
    inTransition = tr;
}

void Cena::setAtivo( bool at ) {
    active = at;
}

void Cena::resetTimer() {
    timeElapsed = 0;
}


void Cena::desligaCena( bool transition ) {
    active = false;
    inTransition = transition;
    resetTimer();
}
