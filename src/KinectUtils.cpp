#include "KinectUtils.h"

void KinectUtils::calculaMassa() {
    depthPixels = depthCam.getPixels();

    int i = 0;
    int qtdMedia = 0;
    depthTotal = 0;
    centroMassa.set(0,0);
    while( i < depthPixels.size() ) {
        int brilho = depthPixels[i];
        if( brilho > 240 ) {
            centroMassa.x += i % 640;
            centroMassa.y += floor(i/640);
            qtdMedia++;
            depthTotal += brilho/255; //normaliza
        }
        i++;
    }
    centroMassa = centroMassa / qtdMedia;
    centroMassa.x = centroMassa.x/640;
    centroMassa.y = centroMassa.y/480;
    depthAvg = depthTotal / qtdMedia;
}
void KinectUtils::loadMask() {
    maskImg.load("../data/mask.png");
}

void KinectUtils::setup() {

    // Variáveis para controlar a visibilidade
    anguloKinect    = 0;
    brilhoKinect    = 0;
    contrasteKinect = 100;
    rastro = 0;
    blur = 0;

    depthAvg = 0; //Média do brilho dos pixels de profundidade

    // Aloca espaço pra imagem grayScale
    depthCam.allocate(640,480);
    maskCam.allocate(640,480);
    floatDepth.allocate(640,480);
    videoDemo.load("../data/depthdance.mp4");
    maskImg.load("../data/mask.png");
    maskColorImg.setFromPixels(maskImg.getPixels());
    maskCam = maskColorImg;
    videoDemo.play();

    fboImgCam.allocate(1024,768);
}

void KinectUtils::drawImg(bool espelho) {
    fboImgCam.begin();
    ofClear(0,0,0,0);
    depthCam.mirror(false, espelho);
    depthCam.draw(0,0,1024,768);
    fboImgCam.end();

    fboImgCam.draw(0,0,1024,768);
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
    } else {
        videoDemo.update();
        imgVideo.setFromPixels(videoDemo.getPixels());
        depthCam = imgVideo;
    }
    // Aplica controles de brilho e contraste
    depthCam.brightnessContrast(brilhoKinect/100., contrasteKinect/100.);


    // Passa pixel a pixel processando por uma floatImg
    ofFloatPixels & pixF = floatDepth.getFloatPixelsRef();
    ofPixels & pixA = depthCam.getPixels();
    ofPixels & pixM = maskCam.getPixels();
    int numPixels = pixF.size();
    for (int i = 0; i < numPixels; i++) {
        // Aplica o Rastro
        pixF[i] = pixF[i] *((float)rastro/100) + (float)pixA[i]/255 * (1. - (float)rastro/100);
        // Aplica a mascára
        pixF[i] = pixF[i] * (float)pixM[i]/255;
    }
    floatDepth.flagImageChanged();

    depthCam = floatDepth;

    if( erodeAndDilate ) {
        depthCam.dilate();
        depthCam.dilate();
        depthCam.dilate();
        depthCam.blur(11);
        depthCam.erode();
        depthCam.erode();
        depthCam.erode();
        depthCam.blur(11);
        depthCam.dilate();
        depthCam.dilate();
        depthCam.dilate();
        depthCam.dilate();
        depthCam.blur(11);
        depthCam.erode();
        depthCam.erode();
        depthCam.erode();
        depthCam.erode();
        depthCam.erode();
        depthCam.blur(11);
    }

    if(blur > 0) {
        depthCam.blur(blur);
    }
    // Calcula centro de massa e depthAvg
    calculaMassa();
}

void KinectUtils::setErodeAndDilate(bool aplica) {
    erodeAndDilate = aplica;
}

void KinectUtils::drawMiniatura(int x,int y,int w,int h) {
    ofSetColor( 255, 255, 255 );
    // Desenha img com filtros aplicados
    depthCam.draw( x, y, w, h);
    //maskImg.draw( x, y, w, h);
    // Desenha centro de massa
    ofSetColor( 255, 0, 255 );
    ofDrawCircle( (centroMassa.x * w) + x, (centroMassa.y * h) + y, w/640+2);
}

void KinectUtils::drawGUI() {
    // Inicia a janela de infos
    ImGui::SetNextWindowSize(ofVec2f(330, 50), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Controles Kinect");
        if (ImGui::Button("Recarregar mascara")) { loadMask(); } 

    // Exibe infos
    ImGui::Text("%.1f FPS (%.3f ms/frame) ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    //Exibe infos
    ImGui::Text("Centro de Massa [x]:%.1f [y]:%.1f", centroMassa.x, centroMassa.y);
    ImGui::Text("Densidade: %.1f", depthAvg);
    ImGui::Text("Massa: %.1f", depthTotal);

    // Botões de liga e desliga do kinect
    if ( kinectGlobal.isConnected() ) {
        ImGui::SliderInt("angulo", &anguloKinect, -30, 30);
        if (ImGui::Button("Desliga Kinect")) { desligaKinect(); } 
    } else {
        if (ImGui::Button("Liga Kinect")) { ligaKinect(); } 
    }

    ImGui::SliderInt("brilho", &brilhoKinect, 0, 100);
    ImGui::SliderInt("contraste", &contrasteKinect, 0, 100);
    ImGui::SliderInt("rastro", &rastro, 0, 100);
    ImGui::SliderInt("blur", &blur, 0, 120);

    // Mostra miniatura e variaveis calculadas
    this->drawMiniatura();

    ImGui::End();
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
