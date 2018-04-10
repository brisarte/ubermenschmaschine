#include "Configs.h"

void Configs::setup() {
    // Inicia o kinect e suas variáiveis de controle
    ku.setup();
    // Inicia a GUI
    configs.setup();
    // Cria cenas que serão exibidas
    cenaEntrada = new CenaEntrada(&ku, true);
    cenaAgua    = new CenaAgua(&ku, false);
    cenaRaizes  = new CenaRaizes(&ku, false);
    // Adiciona cenas
    cenas.push_back(cenaEntrada);
    cenas.push_back(cenaAgua);
    cenas.push_back(cenaRaizes);

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
            cenas[i]->inTransition = false;
            cenas[i]->resetTimer();
            // proxima cena
            int prox = i == cenas.size()-1 ? 0 : i + 1;
            cenas[prox]->setAtivo(true);
        }
    }

    ku.update();
}

void Configs::draw() {
    ofSetColor( 255, 255, 255 );
    configs.begin();
    // Inicia a janela de infos
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles Kinect");
    // Exibe infos
    ImGui::Text("%.1f FPS (%.3f ms/frame) ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

    // Exibe infos e controles do kinect
    ku.drawGUI();

    // Mostra miniatura e variaveis calculadas
    ku.drawMiniatura();

    ImGui::End();

    configs.end();
}

void Configs::mousePressed(int x, int y, int iButton) {
}


