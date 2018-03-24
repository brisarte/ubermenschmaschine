#include "KinectUtils.h"

void KinectUtils::calculaMassa() {
    if( kinectGlobal.isConnected() ) {
        depthPixels = depthCam.getPixels();

        int i = 0;
        int qtdMedia = 0;
        depthAvg = 0;
        centroMassa.set(0,0);
        while( i < depthPixels.size() ) {
            int brilho = depthPixels[i];
            if( brilho > 0 ) {
                depthAvg += brilho;
                centroMassa.x += i % 640;
                centroMassa.y += floor(i/640);
                qtdMedia++;
            }
            i++;
        }
        centroMassa = centroMassa / qtdMedia;
        depthAvg = depthAvg / qtdMedia;
        /*
        centroMassa.x = centroMassa.x / qtdMedia;
        centroMassa.y = centroMassa.y / qtdMedia;
        */
    }
}

void KinectUtils::setup() {
    
    // Variáveis para controlar a visibilidade
    anguloKinect    = 0;
    brilhoKinect    = 50;
    contrasteKinect = 50;

    depthAvg = 0; //Média do brilho dos pixels de profundidade

    // Aloca espaço pra imagem grayScale
    depthCam.allocate(640,480);
}

void KinectUtils::update() {
    // Executa somente se o kinect estiver conectado
    if ( kinectGlobal.isConnected() ) {
        // Posiciona o kinect
        kinectGlobal.setCameraTiltAngle(anguloKinect);
        // Atualiza imagem da câmera
        kinectGlobal.update();
        // Passa a imagem da câmera de profundidade para uma img grayScale
        depthCam.setFromPixels(kinectGlobal.getDepthPixels());
        // Aplica controles de brilho e contraste
        depthCam.brightnessContrast(brilhoKinect/100., contrasteKinect/100.);
        
        // Calcula centro de massa e depthAvg
        calculaMassa();
    }
}

void KinectUtils::drawMiniatura(int x,int y,int w,int h) {
    if ( kinectGlobal.isConnected() ) {
        // Desenha img com filtros aplicados
        depthCam.draw( x, y, w, h);
        // Desenha centro de massa
        ofSetColor( 255, 0, 255 );
        ofDrawCircle( (centroMassa.x/640 * w) + x, (centroMassa.y/480 * h) + y, w/640+2);
    }
}

void KinectUtils::drawGUI() {
    //Exibe infos
    ImGui::Text("Centro de Massa [x]:%.1f [y]:%.1f", centroMassa.x, centroMassa.y);
    ImGui::Text("Profundidade Média: %.1f", depthAvg);
    // Botões de liga e desliga do kinect
    if ( kinectGlobal.isConnected() ) {
        ImGui::SliderInt("angulo", &anguloKinect, -30, 30);
        ImGui::SliderInt("brilho", &brilhoKinect, 0, 100);
        ImGui::SliderInt("contraste", &contrasteKinect, 0, 100);
        if (ImGui::Button("Desliga Kinect")) { desligaKinect(); } 
    } else {
        if (ImGui::Button("Liga Kinect")) { ligaKinect(); } 
    }
 }

void KinectUtils::ligaKinect() {
    if( !kinectGlobal.isConnected() ) {
        kinectGlobal.setRegistration(true);
        kinectGlobal.init();
        kinectGlobal.open();
        kinectGlobal.setCameraTiltAngle(0);
    }
}

void KinectUtils::desligaKinect() {
    kinectGlobal.close();
}
