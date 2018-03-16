#include "Configs.h"

void Configs::setup() {
    configs.setup();
    anguloKinect = 0;
}

void Configs::update() {
    if ( kinectGlobal.isConnected() ) {
        kinectGlobal.update();
    }
}

void Configs::draw() {
    configs.begin();
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles Gerais");
    ImGui::Text("%.1f FPS (%.3f ms/frame) ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

    // Botões de liga e desliga do kinect
    if ( kinectGlobal.isConnected() ) {
        kinectGlobal.setCameraTiltAngle(anguloKinect);
        ImGui::SliderInt("angulo", &anguloKinect, -30, 30);
        if (ImGui::Button("Desliga Kinect")) { desligaKinect(); } 
    } else {
        if (ImGui::Button("Liga Kinect")) { ligaKinect(); } 
    }

    ImGui::End();

    if ( kinectGlobal.isConnected() ) {
        kinectGlobal.drawDepth(5, 5, 320, 240);
    }

    configs.end();
}

void Configs::mousePressed(int x, int y, int iButton) {

}

void Configs::ligaKinect() {
    if( !kinectGlobal.isConnected() ) {
        kinectGlobal.setRegistration(true);
        kinectGlobal.init();
        kinectGlobal.open();
        kinectGlobal.setCameraTiltAngle(0);
    }
}

void Configs::desligaKinect() {
    kinectGlobal.close();
}
