#include "Configs.h"

void Configs::setup() {
    // Inicia o kinect e suas variáiveis de controle
    ku.setup();
    // Inicia a GUI
    configs.setup();
}

void Configs::update() {
    
    ku.update();
}

void Configs::draw() {
    ofSetColor( 255, 255, 255 );
    configs.begin();
    // Inicia a janela de infos
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles Gerais");
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


