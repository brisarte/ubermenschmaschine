#include "Configs.h"

void Configs::setup() {
    // Inicia o kinect e suas variáiveis de controle
    ku.setup();
    // Inicia a GUI
    configs.setup();
    // Cria cenas que serão exibidas
    cenaEntrada     = new CenaEntrada(&ku, true);
    cenaAgua        = new CenaAgua(&ku, false);
    cenaRaizes      = new CenaRaizes(&ku, false);
    cenaSilhueta    = new CenaSilhueta(&ku, false);
    cenaUniverso    = new CenaUniverso(&ku, false);

    // Adiciona cenas
    cenas.push_back(cenaEntrada);
    cenas.push_back(cenaAgua);
    cenas.push_back(cenaRaizes);
    cenas.push_back(cenaSilhueta);
    cenas.push_back(cenaUniverso);

    cenaAtual = 0;

    // Inicializa variável de tempo
    t1 = ofGetElapsedTimef();
}

void Configs::update() {
    // Calcula tempo que passou
    t0 = t1;
    t1 = ofGetElapsedTimef();
    float dt = t1 - t0; 

    for( int i = 0; i < cenas.size(); i++ )
    {
        cenas[i]->update(dt);
        // Passa pra próxima caso tenha acabado a atual
        if(!cenas[i]->active && cenas[i]->inTransition) {
            cenaAtual = i;
            this->proximaCena(cenaAtual);
        }
    }

    ku.update();
}

void Configs::draw() {
    ofSetColor( 255, 255, 255 );
    configs.begin();

    // Inicia a janela de infos
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles  Gerais");

    ImGui::Text("Cena Atual: %i", cenaAtual);
    ImGui::Text("Tempo decorrido: %.1f", cenas[cenaAtual]->timeElapsed);
    if (ImGui::Button("Volta Cena")) { voltaCena(cenaAtual); } 
    if (ImGui::Button("Avança Cena")) { proximaCena(cenaAtual); } 
    if (ImGui::Button("Reinicia Cena")) { resetCena(cenaAtual); } 
    ImGui::End();

    // Exibe infos e controles do kinect
    ku.drawGUI();

    for( int i = 0; i < cenas.size(); i++ )
    {
        cenas[i]->drawConfigs();
    }

    configs.end();
}

void Configs::mousePressed(int x, int y, int iButton) {
}

void Configs::proximaCena(int i) {
    //Força que a centa atual acabe caso ainda nao tenha acabado
    cenas[i]->active = false;
    cenas[i]->inTransition = false;
    cenas[i]->resetTimer();

    // proxima cena
    int prox = i == this->cenas.size()-1 ? 0 : i + 1;
    this->cenas[prox]->setAtivo(true);

    this->cenaAtual = prox;
}

void Configs::voltaCena(int i) {
    //Força que a centa atual acabe caso ainda nao tenha acabado
    cenas[i]->active = false;
    cenas[i]->inTransition = false;
    cenas[i]->resetTimer();

    // proxima cena
    int prox = i == 0 ? this->cenas.size()-1 : i - 1;
    this->cenas[prox]->setAtivo(true);

    this->cenaAtual = prox;
}

void Configs::resetCena(int i) {
    cenas[i]->resetTimer();
}
