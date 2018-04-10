#pragma once

#include "ofxOpenCv.h"

#include "KinectUtils.h"

class Cena {

    public:
        void setup(KinectUtils *kutils, bool ativo = true);
        virtual void update( float dt = 0 );

        // Calcula se a cena está ativa e se deve trocar
        void atualizaTransicoes( float dt = 0 );

        void draw();
        // Funções separadas para desenhar quando está ativo
        virtual void drawAtivo();
        // e para desenhar quando está trocando de cena
        virtual void drawTransicao();

        void setTransicao( bool tr = true );
        void setAtivo( bool at = true );
        void resetTimer();
        KinectUtils *ku;
        ofFbo fboCena;
        ofShader shaderCena;
    
        bool active;
        bool inTransition;
        float timeElapsed;
        float timeStartTransicao;
        float tempoMaximo;
        float tempoTransicao;
};

class CenaEntrada : public Cena {
    public:
        CenaEntrada(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
};

class CenaAgua : public Cena {
    public:
        CenaAgua(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();

        ofImage imgAgua;
};

class CenaRaizes : public Cena {
    public:
        CenaRaizes(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();

        ofVideoPlayer videoRaiz;
};
