#pragma once

#include "ofMain.h"
#include "wires.h"
#include "ofxImageSequenceRecorder.h"
#include "OpenNI.h"
#include "NiTE.h"
#include "OniManager.hpp"

struct Shader {
	ofShader actualShader;
	string fragmentName;
	Shader(ofShader s, string n) : actualShader(s), fragmentName(n) {}
	void reload() {
		this->actualShader.load("identity.vert", this->fragmentName);
	}
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void reloadShaders();
		bool shadersDirty = false;

		static const int WIDTH = 640;
		static const int HEIGHT = 480;
		static const int FPS = 30;

		OniManager oniManager;
		ofImage depthFrame;
		ofImage userFrame;

		glm::vec2 focus;
		Wires wires;
		void drawWires();

		void drawNoise();
		vector<Shader> _shaders;
		Shader* noiseShader = NULL;
		size_t noiseShaderIndex;

		ofxImageSequenceRecorder recorder;
		bool recording = false;
		ofFbo canvas;
		ofRectangle canvasSpace = ofRectangle(0, 0, WIDTH, HEIGHT);
		ofRectangle projectionSpace = ofRectangle(0, 0, WIDTH, HEIGHT);
		void sizeProjectionSpace();
		bool needsResize = true;
};
