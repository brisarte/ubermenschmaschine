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
        void desligaCena( bool transition = false );
        KinectUtils *ku;
        ofFbo fboCena;
        ofShader shaderCena;

        ofSoundPlayer musicaCena;
    
        bool active;
        bool inTransition;
        float timeElapsed;
        float timeStartTransicao;
        float tempoMaximo;
        float tempoTransicao;
        int qtdBlur;
        int qtdRastro;
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

        ofPoint centroNorm;
        int massaMaxima;
        float nivelProporcao;
};

class CenaAgua : public Cena {
    public:
        CenaAgua(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();

        ofShader shaderKinect;
};


class VideoRaiz {
	public:
		ofVideoPlayer video;
		ofVec2f inicio;
        int rotacao;
};

class CenaRaizes : public Cena {
    public:
        CenaRaizes(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void limpaCena();
		void adicionaRaiz(int i);
		void adicionaFlor(int i);

        ofShader shaderFlor;

        vector<VideoRaiz*> videosRaiz;
        vector<VideoRaiz*> videosFlor;
};

class Borboleta {
	public:
        Borboleta(float x = 0, float y = 0);
		ofVideoPlayer video;
		ofVec2f posBorboleta;
        ofImage imgBorboleta;
};

class CenaBorboleta : public Cena {
    public:
        CenaBorboleta(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void limpaCena();
		void criaBorboletas();

        ofShader shaderSilhueta;
        ofFbo    fboFundo;

        vector<Borboleta*> panapana;

        ofVideoPlayer fundo;
};

class CenaAbismo : public Cena {
    public:
        CenaAbismo(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void limpaCena();

        ofVideoPlayer video;
};

class CenaNascendo : public Cena {
    public:
        CenaNascendo(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void limpaCena();

        ofVideoPlayer video;
};

class CenaCorpos : public Cena {
    public:
        CenaCorpos(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
		void limpaCena();

        ofVideoPlayer video;
};

class Particula {
    public:
        Particula(float x,float y);
        ofVec2f pos;
        ofVec2f velocidade;
        ofColor cor;

        void update(float dt, ofVec2f aceleracao);
        void draw();
};

class CenaSilhueta : public Cena {
    public:
        CenaSilhueta(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();

        void criaParticulasKinect( ofPixels depthPixels );
        
        int gravidade;

        ofVec2f posEgo;
        ofVec2f aceleracaoEgo;

        vector<Particula*> particulas;
};


class CenaUniverso : public Cena {
    public:
        CenaUniverso(KinectUtils *kutils, bool ativo = true);
        void filtraImg();
        void update( float dt );
        void drawAtivo();
        void drawTransicao();
        void drawConfigs();
        void limpaCena();

        ofVideoPlayer videoUniverso;
        ofFbo fboUniverso;
};

