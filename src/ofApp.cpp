#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	focus = glm::vec2(ofGetWidth() / 2, ofGetHeight()/2);
	ofSetFrameRate(24);

	recorder.setPrefix(ofToDataPath("video_"));
	recorder.setFormat("bmp");
	//ofAddListener(recorder.outputFileCompleteEvent, this, )
	canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	noiseShader.load("identity.vert", "no.frag");

	oniManager.setup(640, 480, 30);
	depthFrame.allocate(640, 480, OF_IMAGE_GRAYSCALE);
}

void ofApp::reloadShaders() {
	if (shadersDirty) {
		ofLogNotice() << "Reloading shader." << endl;
		noiseShader.load("identity.vert", "no.frag");
		GLint err = glGetError();
		if (err != GL_NO_ERROR) {
			ofLogNotice() << "Shader failed to compile:" << endl << err << endl;
		}
		shadersDirty = false;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	focus = glm::vec2(ofGetMouseX(), ofGetMouseY());
	oniManager.getDepthFrame(&depthFrame);
	reloadShaders();
}

void ofApp::drawWires() {
	ofBackgroundGradient(ofColor(200, 120, 0), ofColor::black, OF_GRADIENT_CIRCULAR);
	ofSetColor(0);
	wires.drawAround(focus, 2000);
}

//--------------------------------------------------------------
void ofApp::draw(){
	//canvas.begin();
	    //drawWires();
		ofBackground(0);
		noiseShader.begin();
		noiseShader.setUniform1i("frameNumber", ofGetFrameNum());
		noiseShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		noiseShader.setUniform1f("timetime", ofGetElapsedTimeMillis());
		ofSetColor(255);
		//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		depthFrame.draw(0, 0);
		noiseShader.end();
	//canvas.end();
	//ofSetColor(255);
	//canvas.draw(0, 0);

	if (recording) {
		ofPixels px;
		canvas.readToPixels(px);
		recorder.addFrame(px);
		ofSetColor(ofColor::red);
		ofDrawCircle(20, 20, 10);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'r') {
		if (!recording) {
			recorder.startThread(false);
		}
		if (recording) {
			recorder.stopThread();
		}
		recording = !recording;
	}
	if (key == ' ') {
		shadersDirty = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
