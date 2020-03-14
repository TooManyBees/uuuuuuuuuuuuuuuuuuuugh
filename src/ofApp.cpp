#include "ofApp.h"

const string SHADER_NAME = "no.frag";

//--------------------------------------------------------------
void ofApp::setup(){
	focus = glm::vec2(ofGetWidth() / 2, ofGetHeight()/2);
	ofSetFrameRate(24);

	recorder.setPrefix(ofToDataPath("video_"));
	recorder.setFormat("bmp");
	//ofAddListener(recorder.outputFileCompleteEvent, this, )
	canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	noiseShader.load("identity.vert", SHADER_NAME);
	//noiseShader2.load("identity.vert", "no2.frag");

	oniManager.setup(WIDTH, HEIGHT, FPS);
	depthFrame.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
}

void ofApp::reloadShaders() {
	if (shadersDirty) {
		ofLogNotice() << "Reloading shader." << endl;
		noiseShader.load("identity.vert", SHADER_NAME);
		//noiseShader2.load("identity.vert", "no2.frag");
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
	if (needsResize) sizeProjectionSpace();
}

void ofApp::drawWires() {
	ofBackgroundGradient(ofColor(200, 120, 0), ofColor::black, OF_GRADIENT_CIRCULAR);
	ofSetColor(0);
	//wires.drawAround(focus, 2000);
	auto positions = oniManager.getUserData();
	positions.push_back(focus);
	//cout << "[ ";
	//for (auto &pos : positions) {
	//	cout << "(" << pos << ") ";
	//}
	//cout << "]" << endl;
	wires.drawAroundMulti(positions, 2000);
}

void ofApp::drawNoise() {
	ofBackground(0);
	noiseShader.begin();
		noiseShader.setUniform1i("frameNumber", ofGetFrameNum());
		noiseShader.setUniform2f("resolution", WIDTH, HEIGHT);
		noiseShader.setUniform1f("timetime", ofGetElapsedTimeMillis());
		ofSetColor(255);
		//ofDrawRectangle(canvasSpace);
		depthFrame.draw(canvasSpace);
	noiseShader.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	canvas.begin();
	    //drawWires();
		drawNoise();
	canvas.end();
	ofSetColor(255);
	canvas.draw(projectionSpace);

	if (recording) {
		ofPixels px;
		canvas.readToPixels(px);
		recorder.addFrame(px);
		ofSetColor(ofColor::red);
		ofDrawCircle(20, 20, 10);
		ofSetColor(ofColor::white);
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
	if (key == 'f') {
		ofToggleFullscreen();
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
	needsResize = true;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::sizeProjectionSpace() {
	ofRectangle window = ofGetWindowRect();
	if (window.width > 0 && window.height > 0) {
		projectionSpace.scaleTo(window, OF_SCALEMODE_FIT);
		projectionSpace.alignTo(window, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
	}
	needsResize = false;
}
