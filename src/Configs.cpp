#include "Configs.h"

void Configs::setup() {
    configs.setup();
    anguloKinect    = 0;
    brilhoKinect    = 50;
    contrasteKinect = 50;

    depthMedia      = 0;
    depthCam.allocate(640,480);
}

void Configs::update() {
    if ( kinectGlobal.isConnected() ) {
        // Atualiza imagem da câmera
        kinectGlobal.update();
        // Passa a imagem da câmera de profundidade para uma img
        depthCam.setFromPixels(kinectGlobal.getDepthPixels());
        depthCam.brightnessContrast(brilhoKinect/100., contrasteKinect/100.);
        
        // Calcula centro de massa e depthMedia
        calculaMassa();
    }
}

void Configs::draw() {
    configs.begin();
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles Gerais");
    ImGui::Text("%.1f FPS (%.3f ms/frame) ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("Centro de Massa [x]:%.1f [y]:%.1f", centroMassa.x, centroMassa.y);
    ImGui::Text("Profundidade Média: %.1f", depthMedia);

    // Botões de liga e desliga do kinect
    if ( kinectGlobal.isConnected() ) {
        kinectGlobal.setCameraTiltAngle(anguloKinect);
        ImGui::SliderInt("angulo", &anguloKinect, -30, 30);
        ImGui::SliderInt("brilho", &brilhoKinect, 0, 100);
        ImGui::SliderInt("contraste", &contrasteKinect, 0, 100);
        if (ImGui::Button("Desliga Kinect")) { desligaKinect(); } 
    } else {
        if (ImGui::Button("Liga Kinect")) { ligaKinect(); } 
    }

    ImGui::End();

    if ( kinectGlobal.isConnected() ) {
        depthCam.draw(5, 5, 320, 240);
        ofSetColor( 255, 0, 255 );
        ofDrawCircle(centroMassa.x/2 + 5, centroMassa.y/2 + 5, 5);
        ofSetColor( 255, 255, 255 );
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

void Configs::calculaMassa() {
    if( kinectGlobal.isConnected() ) {
        depthPixels = depthCam.getPixels();

        int i = 0;
        int qtdMedia = 0;
        depthMedia = 0;
        centroMassa.set(0,0);
        while( i < depthPixels.size() ) {
            int brilho = depthPixels[i];
            if( brilho > 0 ) {
                depthMedia += brilho;
                centroMassa.x += i % 640;
                centroMassa.y += floor(i/640);
                qtdMedia++;
            }
            i++;
        }
        centroMassa = centroMassa / qtdMedia;
        depthMedia = depthMedia / qtdMedia;
        /*
        centroMassa.x = centroMassa.x / qtdMedia;
        centroMassa.y = centroMassa.y / qtdMedia;
        */
    }
}
