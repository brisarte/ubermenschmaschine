#include "ofMain.h"
#include "ofApp.h"
#include "Configs.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;

	settings.width = 1024;
	settings.height = 768;
	settings.setPosition(ofVec2f( 1940,100));
	settings.resizable = true;
	settings.decorated = false;
	settings.multiMonitorFullScreen = true;
	settings.numSamples = 8;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	settings.width = 1800;
	settings.height = 950;
	settings.setPosition(ofVec2f( 0,0));
	settings.resizable = true;
	settings.decorated = true;
	settings.multiMonitorFullScreen = false;
	settings.numSamples = 8 ;
	settings.shareContextWith = mainWindow;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<Configs> configs(new Configs);

	mainApp->configs = configs;
	mainApp->window = mainWindow;
	mainApp->gl = dynamic_pointer_cast<ofBaseGLRenderer>(mainWindow->renderer());

	ofRunApp(guiWindow, configs);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
