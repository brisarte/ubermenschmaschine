#pragma once

#include "ofxOpenCv.h"

#include "KinectUtils.h"

class Cena {
    public:
        void setup(KinectUtils *kutils, bool ativo = true);
        virtual void update( float dt = 0 );
        virtual void drawConfigs();

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
        void drawConfigs();
        
        ofShader shaderFiltraImg;
        ofFbo fboCarne;
        ofImage imgCarne;
};

class CenaAgua : public Cena {
    public:
        CenaAgua(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();

        int qtdBlur;
        ofShader shaderKinect;
};


class VideoRaiz {
	public:
		ofVideoPlayer video;
		ofVec2f inicio;
};

class CenaRaizes : public Cena {
    public:
        CenaRaizes(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void adicionaRaiz();

        vector<VideoRaiz*> videosRaiz;
};

class CenaBorboleta : public Cena {
    public:
        CenaBorboleta(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
};

class CenaSol : public Cena {
    public:
        CenaSol(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
};

class CenaSilhueta : public Cena {
    public:
        CenaSilhueta(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
};


class CenaUniverso : public Cena {
    public:
        CenaUniverso(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();

        ofVideoPlayer videoUniverso;
        ofFbo fboUniverso;
};

